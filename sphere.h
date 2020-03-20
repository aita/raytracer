#ifndef _SPHERE_H_
#define _SPHERE_H_
#include <Eigen/Dense>
#include <cmath>

#include "hittable.h"

class Sphere : public Hittable {
  Eigen::Vector3f center_;
  float radius_;

 public:
  Sphere(Eigen::Vector3f center, float r) : center_(center), radius_(r) {}
  virtual bool hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
};

bool Sphere::hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const {
  Eigen::Vector3f oc = r.origin() - center_;
  float a = r.direction().dot(r.direction());
  float b = oc.dot(r.direction());
  float c = oc.dot(oc) - radius_ * radius_;
  float discriminant = b * b - a * c;
  if (discriminant > 0) {
    float temp = (-b - std::sqrt(discriminant)) / a;
    if (temp < tmax && temp > tmin) {
      rec.t = temp;
      rec.p = r.pointAtParameter(rec.t);
      rec.normal = (rec.p - center_) / radius_;
      return true;
    }
    temp = (-b + std::sqrt(discriminant)) / a;
    if (temp < tmax && temp > tmin) {
      rec.t = temp;
      rec.p = r.pointAtParameter(rec.t);
      rec.normal = (rec.p - center_) / radius_;
      return true;
    }
  }
  return false;
}

#endif