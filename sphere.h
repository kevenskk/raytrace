#ifndef SPHERE_H
#define SPHERE_H

#include "hittableobjects.h"
#include "vec3.h"

class sphere : public hittable {
  public:
    sphere(point3 _centre, double _radius, shared_ptr<material> material) : centre(_centre), radius(_radius), m(material) {}

    bool hit(const ray& r, interval tmintmax, hitRecord& rec) const override {
        vec3 oc = r.origin() - centre;
        auto A = r.direction().length_squared();
        auto halfB = dot(oc, r.direction());
        auto C = oc.length_squared() - radius*radius;

        auto discriminant = halfB*halfB - A*C;



        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);

        auto root = (-halfB - sqrtd) / A;
            if (!tmintmax.surrounds(root)) {
            root = (-halfB + sqrtd) / A;
            if (!tmintmax.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.pointAtT(rec.t);
        vec3 outward_normal = (rec.p - centre) / radius;
        rec.set_face_normal(r, outward_normal);
        getUV(outward_normal, rec.u, rec.v);

        rec.m = m;
        return true;
    }

  private:
   
    point3 centre;
    double radius;
    shared_ptr<material> m;


    static void getUV(const point3& p,double& u, double& v){

      // return 2d/uv coordinates of a given point on the sphere
      auto theta = acos(-p.y());
      auto phi = atan2(-p.z(), p.x()) + pi;

      u = phi / (2*pi);
      v = theta/ pi;
    }
};

#endif