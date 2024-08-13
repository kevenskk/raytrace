#ifndef STB_IMAGE_H
#define STB_IMAGE_H

#ifdef _MSC_VER
    #pragma warning (push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"
#include "helper.h"

#include <cstdlib>
#include <iostream>

class stbimage {
  public:
    stbimage() : pixelData(nullptr) {}

    stbimage(const char* image_filename) {
     

        auto filename = std::string(image_filename);
        auto imagedir = getenv("IMAGES");

        if (imagedir && load(std::string(imagedir) + "/" + image_filename)) return;
        if (load(filename)) return;
        if (load("assets/" + filename)) return;
    

        std::cerr << "File not found: '" << image_filename << "'.\n";
    }

    ~stbimage() { STBI_FREE(pixelData); }

    bool load(const std::string filename) {

        auto n = bytes_per_pixel; 
        pixelData = stbi_load(filename.c_str(), &imgwidth, &imgheight, &n, bytes_per_pixel);
        bytes_per_scanline = imgwidth * bytes_per_pixel;
        return pixelData != nullptr;
    }

    int width()  const { return (pixelData == nullptr) ? 0 : imgwidth; }
    int height() const { return (pixelData == nullptr) ? 0 : imgheight; }

    const unsigned char* pixel_data(int x, int y) const {
        static unsigned char colour[] = { 255, 0, 255 };
        if (pixelData == nullptr) return colour;;

        x = clamp(x, 0, imgwidth);
        y = clamp(y, 0, imgheight);

        return pixelData + y*bytes_per_scanline + x*bytes_per_pixel;
    }

  private:
    const int bytes_per_pixel = 3;
    unsigned char *pixelData;
    int imgwidth, imgheight;
    int bytes_per_scanline;

    
};

#ifdef _MSC_VER
    #pragma warning (pop)
#endif

#endif