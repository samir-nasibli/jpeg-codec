#include "image.h"

#include <jpeglib.h>

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>
#include <thread>

#include "tbb/parallel_for.h"
#include "tbb/blocked_range2d.h"
#include "tbb/task_scheduler_init.h"

#include <tbb/tick_count.h>


namespace jpegImage
{
Image::Image( const std::string& fileName )
{
    // Creating a custom deleter for the decompressInfo pointer
    // to ensure ::jpeg_destroy_compress() gets called even if
    // we throw out of this function.
    auto dt = []( ::jpeg_decompress_struct *ds )
            {
                ::jpeg_destroy_decompress( ds );
            };
    std::unique_ptr<::jpeg_decompress_struct, decltype(dt)> decompressInfo(
            new ::jpeg_decompress_struct,
            dt
            );

    m_errorMgr = std::make_shared<::jpeg_error_mgr>();

    // Using fopen here ( and in save() ) because libjpeg expects
    // a FILE pointer.
    // FILE*  is stored in a unique_ptr. 
    // using custom deleter here to ensure fclose() gets called even if we throw.
    auto fdt = []( FILE* fp )
            {
                fclose( fp );
            };
    std::unique_ptr<FILE, decltype(fdt)> infile(
            fopen( fileName.c_str(), "rb" ),
            fdt
            );
    if ( infile.get() == NULL )
    {
        throw std::runtime_error( "Could not open " + fileName );
    }

    decompressInfo->err = ::jpeg_std_error( m_errorMgr.get() );
    
    m_errorMgr->error_exit = [](::j_common_ptr cinfo)
        {
            char jpegLastErrorMsg[JMSG_LENGTH_MAX];
            // Call the function pointer to get the error message
            ( *( cinfo->err->format_message ) )
                ( cinfo, jpegLastErrorMsg );
            throw std::runtime_error( jpegLastErrorMsg );
        };
    ::jpeg_create_decompress( decompressInfo.get() );

    // Read the file:
    ::jpeg_stdio_src( decompressInfo.get(), infile.get() );

    int rc = ::jpeg_read_header( decompressInfo.get(), TRUE );
    if (rc != 1)
    {
        throw std::runtime_error(
            "File does not seem to be a normal JPEG"
            );
    }
    ::jpeg_start_decompress( decompressInfo.get() );

    m_width       = decompressInfo->output_width;
    m_height      = decompressInfo->output_height;
    m_pixelSize   = decompressInfo->output_components;
    m_colourSpace = decompressInfo->out_color_space;

    size_t row_stride = m_width * m_pixelSize;

    m_bitmapData.clear();

    m_bitmapData.resize( m_pixelSize * m_width * m_height );

    uint8_t * ptr = m_bitmapData.data();

    while ( decompressInfo->output_scanline < m_height )
    {
        ::jpeg_read_scanlines( decompressInfo.get(), &ptr, 1 );
        ptr +=row_stride;
    }

    ::jpeg_finish_decompress( decompressInfo.get() );
}

// Copy constructor
Image::Image( const Image& rhs )
{
    m_errorMgr      = rhs.m_errorMgr;
    m_bitmapData    = rhs.m_bitmapData;
    m_width         = rhs.m_width;
    m_height        = rhs.m_height;
    m_pixelSize     = rhs.m_pixelSize;
    m_colourSpace   = rhs.m_colourSpace;
}

Image::~Image()
{
}

void Image::save( const std::string& fileName, int quality ) const
{
    if ( quality < 0 )
    {
        quality = 0;
    }
    if ( quality > 100 )
    {
        quality = 100;
    }
    FILE* outfile = fopen( fileName.c_str(), "wb" );
    if ( outfile == NULL )
    {
        throw std::runtime_error(
            "Could not open " + fileName + " for writing"
            );
    }
    // Creating a custom deleter for the compressInfo pointer
    // to ensure ::jpeg_destroy_compress() gets called even if
    // we throw out of this function.
    auto dt = []( ::jpeg_compress_struct *cs )
            {
                ::jpeg_destroy_compress( cs );
            };
    std::unique_ptr<::jpeg_compress_struct, decltype(dt)> compressInfo(
            new ::jpeg_compress_struct,
            dt );

    ::jpeg_create_compress( compressInfo.get() );
    ::jpeg_stdio_dest( compressInfo.get(), outfile);
    compressInfo->image_width = m_width;
    compressInfo->image_height = m_height;
    compressInfo->input_components = m_pixelSize;
    compressInfo->in_color_space =
        static_cast<::J_COLOR_SPACE>( m_colourSpace );
    compressInfo->err = ::jpeg_std_error( m_errorMgr.get() );
    ::jpeg_set_defaults( compressInfo.get() );
    ::jpeg_set_quality( compressInfo.get(), quality, TRUE );
    ::jpeg_start_compress( compressInfo.get(), TRUE);

    size_t row_stride = m_width * m_pixelSize;

    ::JSAMPROW row_pointer[1]; /* pointer to JSAMPLE row[s] */

    while (compressInfo->next_scanline < compressInfo->image_height) {

        row_pointer[0] = const_cast<::JSAMPROW> (& m_bitmapData[compressInfo->next_scanline * row_stride]);
        ::jpeg_write_scanlines(compressInfo.get(), row_pointer, 1);

    }
    
    ::jpeg_finish_compress( compressInfo.get() );
    fclose( outfile );
    std::cout << fileName <<" image has been successfully saved " <<std::endl;
}

// prints some image properties: height, width and pixel size
void Image::printImageProperties() const
{
    std::cout << "\nImage height: " << m_height
            << "\nImage width : " << m_width
            << "\nImage px sz : " << m_pixelSize
            << std::endl;
}

void Image::invert()
{  
    tbb::tick_count t0 = tbb::tick_count::now();

    for (std::vector<uint8_t>::iterator it = m_bitmapData.begin(); it != m_bitmapData.end(); ++it)
    {
        *it = 255 - *it;
    }

    // Print time results
    tbb::tick_count t1 = tbb::tick_count::now();
    std::cout<<"invert succesuflly: " << (t1-t0).seconds()<< " seconds"<<std::endl;
}

void Image::parallelInvert() {
    
    size_t m = m_bitmapData.size();

    size_t p = tbb::task_scheduler_init::default_num_threads();

    // The computation 2*n/p-1 here is because in TBB, 
    // grainsize is not a minimal size of a possible 
    // sub-range but the threshold used to decide whether to split.
    size_t grainsize = 2*m/p-1;

    tbb::tick_count t0 = tbb::tick_count::now();

    tbb::parallel_for(
        tbb::blocked_range<size_t>(0,m,grainsize),
        [&] (tbb::blocked_range<size_t> r) {
            for (size_t i = r.begin(); i != r.end(); ++i )
                {
                    m_bitmapData[i] = 255 - m_bitmapData[i];
                }
        });

    // Print time results
    tbb::tick_count t1 = tbb::tick_count::now();
    std::cout<<"parallelInvert succesuflly: " << (t1-t0).seconds()<< " seconds"<<std::endl;
}

void Image::stdInvert()
{
    typedef std::vector<uint8_t> container;
    typedef container::iterator iter;

    auto loop = [] (iter begin, iter end)
    {
        for(auto it = begin; it != end; ++it)
            *it = 255 - *it;
    };

    tbb::tick_count t0 = tbb::tick_count::now();

    // parallel
    size_t m = m_bitmapData.size();
    
    //size_t threadsNumber = (m > 5) ? 5 : 1;

    //temporary using tbb func: number of threads
    size_t threadsNumber = tbb::task_scheduler_init::default_num_threads();

    size_t grainsize = m/threadsNumber;

    
    std::vector<std::thread> threads(threadsNumber);

    auto loop_iter = std::begin(m_bitmapData);
    
    for(auto it = std::begin(threads); it != std::end(threads) - 1; ++it)
    {
        *it = std::thread(loop, loop_iter, loop_iter + grainsize);
        loop_iter += grainsize;
    }
    
    threads.back() = std::thread(loop, loop_iter, std::end(m_bitmapData));

    for(auto& i : threads)
    {
        i.join();
    }

    // Print time results
    tbb::tick_count t1 = tbb::tick_count::now();
    std::cout<<"stdInvert succesuflly:\t" << (t1-t0).seconds()<< " seconds"<<std::endl;
}
} // jpegImage namespace
