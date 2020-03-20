#ifndef DIELECTRIC_H_
#define DIELECTRIC_H_
#include <cmath>

#include "material.h"
#include "random.h"

float schlick(float cosine, float refraction_index) {
  float r0 = (1.f - refraction_index) / (1.f + refraction_index);
  r0 = r0 * r0;
  return r0 + (1.f - r0) * std::pow((1 - cosine), 5.f);
}

class Dielectric : public Material {
 public:
  Dielectric(float refractive_index) : refractive_index_(refractive_index) {}

  virtual bool scatter(const Ray& r_in,
                       const HitRecord& rec,
                       Eigen::Vector3f& attenuation,
                       Ray& scattered) const {
    Eigen::Vector3f outward_normal;
    Eigen::Vector3f reflected = reflect(r_in.direction(), rec.normal);
    float ni_over_nt;
    attenuation = Eigen::Vector3f::Ones();
    Eigen::Vector3f refracted;

    float reflect_prob;
    float cosine;

    if (r_in.direction().dot(rec.normal) > 0.f) {
      outward_normal = -rec.normal;
      ni_over_nt = refractive_index_;
      cosine = refractive_index_ * r_in.direction().dot(rec.normal) /
               r_in.direction().norm();
    } else {
      outward_normal = rec.normal;
      ni_over_nt = 1.f / refractive_index_;
      cosine = -r_in.direction().dot(rec.normal) / r_in.direction().norm();
    }

    if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
      reflect_prob = schlick(cosine, refractive_index_);
    } else {
      reflect_prob = 1.f;
    }

    if (Random<double>() < reflect_prob) {
      scattered = Ray(rec.p, reflected);
    } else {
      scattered = Ray(rec.p, refracted);
    }

    return true;
  }

 private:
  float refractive_index_;
};

#endif
