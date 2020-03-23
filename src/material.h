#ifndef MATERIAL_H_
#define MATERIAL_H_
#include <Eigen/Dense>

#include "hittable.h"
#include "random.h"

Eigen::Vector3f reflect(const Eigen::Vector3f& v, const Eigen::Vector3f& n) {
  return v - 2.f * v.dot(n) * n;
}

bool refract(const Eigen::Vector3f& v,
             const Eigen::Vector3f& n,
             float ni_over_nt,
             Eigen::Vector3f& refracted) {
  Eigen::Vector3f uv = v.normalized();
  float dt = uv.dot(n);
  float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1.f - dt * dt);
  if (discriminant > 0) {
    refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
    return true;
  }
  return false;
}

float schlick(float cosine, float refraction_index) {
  float r0 = (1.f - refraction_index) / (1.f + refraction_index);
  r0 = r0 * r0;
  return r0 + (1.f - r0) * std::pow((1 - cosine), 5.f);
}

class Material {
 public:
  virtual bool scatter(const Ray& r_in,
                       const HitRecord& rec,
                       Eigen::Vector3f& attenuation,
                       Ray& scatterd) const = 0;
};

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

class Metal : public Material {
 public:
  Metal(const Eigen::Vector3f& albedo, float fuzziness)
      : albedo_(albedo), fuzziness_(std::clamp(fuzziness, 0.f, 1.f)) {}

  virtual bool scatter(const Ray& r_in,
                       const HitRecord& rec,
                       Eigen::Vector3f& attenuation,
                       Ray& scattered) const {
    auto reflected = reflect(r_in.direction().normalized(), rec.normal);
    scattered = Ray(rec.p, reflected + fuzziness_ * RandomInUnitSphere());
    attenuation = albedo_;
    return scattered.direction().dot(rec.normal) > 0.f;
  }

 private:
  Eigen::Vector3f albedo_;
  float fuzziness_;
};

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