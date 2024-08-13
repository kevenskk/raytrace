#ifndef RAY_H
#define RAY_H

#include "vec3.h"

struct ray {
  public:
    ray() {}

    ray(const point3& origin, const vec3& direction) : o(origin), d(direction) {}

    inline point3 origin() const  { return o; }
    inline vec3 direction() const { return d; }


    point3 pointAtT(double t) const {
        return o + t*d;
    }
    

  private:
    point3 o;
    vec3 d;
    float tMin = 0.001;
    float tMax = infinity;
};

#endif