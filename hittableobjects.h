#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "interval.h"



class material;

class hitRecord {
  public:
    point3 p;
    vec3 normal;
    double t;
    double u;
    double v;
    shared_ptr<material> m;

     bool frontFace;

    void set_face_normal(const ray& r, const vec3& outwardNormal) {
    

        frontFace = dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class hittable {
  public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, interval tmintmax, hitRecord& rec) const = 0;
};

#endif