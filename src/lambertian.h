#ifndef LAMBERTIAN_H_
#define LAMBERTIAN_H_
#include <Eigen/Dense>

#include "material.h"
#include "random.h"

class Lambertian : public Material {
 public:
  Lambertian(const Eigen::Vector3f& albedo) : albedo_(albedo) {}

  virtual bool scatter(const Ray& r_in,
                       const HitRecord& rec,
                       Eigen::Vector3f& attenuation,
                       Ray& scattered) const {
    auto target = rec.p + rec.normal + RandomInUnitSphere();
    scattered = Ray(rec.p, target - rec.p);
    attenuation = albedo_;
    return true;
  }

 private:
  Eigen::Vector3f albedo_;
};

#endif