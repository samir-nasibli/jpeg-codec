
#ifndef IMAGE_H
#define IMAGE_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

struct jpeg_error_mgr;

namespace jpegImage
{

class Image
{
public:

    // Will throw if file cannot be loaded, or is in the wrong format,
    // or some other error is encountered.
    explicit Image( const std::string& fileName );

    // Construct from an existing image object. This allows
    // to work on a copy  without affecting the
    // original we have in memory.
    Image( const Image& rhs );

    // Assigment and move operations are disallowed
    Image& operator=( const Image& ) = delete;
    Image( Image&& )                 = delete;
    Image& operator=( Image&& )      = delete;

    ~Image();

    // Will throw if file cannot be saved.
    // Quality's usable values are 0-100
    void save( const std::string& fileName, int quality = 95 ) const;

    size_t getHeight()    const { return m_height; }
    size_t getWidth()     const { return m_width;  }
    size_t getPixelSize() const { return m_pixelSize; }

    // prints some image properties: height, width and pixel size
    void printImageProperties() const; 

    // inverts pixels of image (no multithread)
    void invert();
    
    // inverts pixels of image (TBB)
    void parallelInvert();

    // inverts pixels of image (uses stl thread)
    void stdInvert();

private:
    std::shared_ptr<::jpeg_error_mgr> m_errorMgr;
    std::vector<uint8_t>              m_bitmapData;
    size_t                            m_width;
    size_t                            m_height;
    size_t                            m_pixelSize;
    int                               m_colourSpace;

};
} // jpegImage namespace

#endif // IMAGE_H