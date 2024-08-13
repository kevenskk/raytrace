#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"
#include "helper.h"
#include <iostream>

using colour = vec3;






void drawColour(std::ostream &out, colour pixelColour, int spp) {
    auto r = pixelColour.r();
    auto g = pixelColour.g();
    auto b = pixelColour.b();

    const float scale = 1.0 / spp; // scale resulting rgb values by number of samples

    r = sqrt(r*scale); // linear to gamma space correction
    g = sqrt(g*scale);
    b = sqrt(b*scale);
    
    out << static_cast<int>(256 * clamp(r,0.000, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g,0.000, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b,0.000, 0.999)) << '\n';


        
}

#endif