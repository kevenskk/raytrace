#ifndef TEXTURE_H
#define TEXTURE_H

#include "helper.h"
#include "colour.h"
#include "stbimage.h"

class texture {
  public:
    virtual ~texture() = default;

    virtual colour value(double u, double v, const point3& p) const = 0;
};

class solid_colour : public texture {
  public:
    solid_colour(colour c) : colour_value(c) {}

    solid_colour(double red, double green, double blue) : solid_colour(colour(red,green,blue)) {}

    colour value(double u, double v, const point3& p) const override {
        return colour_value;
    }

  private:
    colour colour_value;
};

class surfacetexture : public texture {
  public:
    surfacetexture(const char* filename) : image(filename) {}

    colour value(double u, double v, const point3& p) const override {
        if (image.height() <= 0) return colour(0,1,1);

        u = interval(0,1).clamp(u);
        v = 1.0 - interval(0,1).clamp(v);  

        auto i = static_cast<int>(u * image.width());
        auto j = static_cast<int>(v * image.height());
        auto pixel = image.pixel_data(i,j);

        auto scale = 1.0 / 255.0;
        return colour(scale*pixel[0], scale*pixel[1], scale*pixel[2]);
    }

  private:
    stbimage image;
};



#endif