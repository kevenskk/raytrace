#ifndef MATERIAL_H
#define MATERIAL_H

#include "helper.h"
#include "ray.h"
#include "colour.h"
#include "hittablelist.h"
#include "texture.h"

class hit_record;

class material {
  public:
    virtual ~material() = default;
    virtual colour emitted(double u, double v, const point3& p) const {
        return colour(0,0,0);
    }
    virtual bool scatter(
        const ray& r, const hitRecord& rec, colour& attenuation, ray& scattered) const = 0;
};

class diffuse : public material { // ideal diffuse material (Lambertian)
  public:
    diffuse(const colour& a) : albedo(make_shared<solid_colour>(a)) {}
    diffuse(shared_ptr<texture> a) : albedo(a) {}

    void setAlbedo(const colour& newAlbedo) {
        //albedo = newAlbedo;
    }
    bool scatter(const ray& r, const hitRecord& rec, colour& attenuation, ray& scattered)
    const override {
        auto scatter_direction = rec.normal + RandomUnitVector();

        if (scatter_direction.veczero())
            scatter_direction = rec.normal;
        

        scattered = ray(rec.p, scatter_direction);

        
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

  private:
        shared_ptr<texture> albedo;

};

class metal : public material { // metal material 
  public:
    metal(const colour& a, const float& r) : albedo(a), roughness(r < 1 ? r : 1) {}
    void setAlbedo(const colour& newAlbedo) {
        albedo = newAlbedo;
    }
    bool scatter(const ray& r, const hitRecord& rec, colour& attenuation, ray& scattered)
    const override {

        vec3 perturbedN = rec.normal + roughness * RandomUnitVector(); // normal is pertubed instead of reflecting perfectly
        vec3 reflected = reflect(normalize(r.direction()),perturbedN);

        scattered = ray(rec.p, reflected);

        attenuation = albedo;

        return (dot(scattered.direction(), rec.normal) > 0);
    }

  private:
    colour albedo;
    double roughness;
};

class refractive : public material { // refractive material based on Snell's law
  public:
    refractive(const colour a, double n1, double n2) : albedo(a), n1(n1), n2(n2) {} // refracive indices of the two media
    void setAlbedo(const colour& newAlbedo) {
        albedo = newAlbedo;
    }
    bool scatter(const ray& r, const hitRecord& rec, colour& attenuation, ray& scattered)
    const override {

        attenuation = albedo;
        double n1_over_n2 = rec.frontFace ? (1.0/n2) : n2; // ratio of the two indices

        vec3 refracted = refract(r.direction(), rec.normal, n1_over_n2); // generate refracted ray

        scattered = ray(rec.p, refracted); // scattered ray is computed as the new ray
        return true;
    }

  private:
    double n1; // Index of Refraction
    double n2;
    colour albedo;
};





class emissive : public material {
  public:
    emissive(shared_ptr<texture> a) : emit(a) {}
    emissive(colour c) : emit(make_shared<solid_colour>(c)) {}
    
    bool scatter(const ray& r_in, const hitRecord& rec, colour& attenuation, ray& scattered)
    const override {
        return false;
    }

    colour emitted(double u, double v, const point3& p) const override {
        return emit->value(u, v, p);
    }

  private:
    shared_ptr<texture> emit;
};






#endif