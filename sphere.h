#ifndef SPHERE_H_
#define SPHERE_H_
#include <Eigen/Dense>
#include <cmath>

#include "hittable.h"

class Sphere : public Hittable {
 public:
  Sphere(Eigen::Vector3f center, float r) : center_(center), radius_(r) {}
  virtual bool hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;

 private:
  Eigen::Vector3f center_;
  float radius_;
};

bool Sphere::hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const {
  Eigen::Vector3f oc = r.origin() - center_;
  float a = r.direction().dot(r.direction());
  float b = oc.dot(r.direction());
  float c = oc.dot(oc) - radius_ * radius_;
  float discriminant = b * b - a * c;
  if (discriminant > 0) {
    float tmp = (-b - std::sqrt(discriminant)) / a;
    if (tmp < tmax && tmp > tmin) {
      rec.t = tmp;
      rec.p = r.pointAtParameter(rec.t);
      rec.normal = (rec.p - center_) / radius_;
      return true;
    }
    tmp = (-b + std::sqrt(discriminant)) / a;
    if (tmp < tmax && tmp > tmin) {
      rec.t = tmp;
      rec.p = r.pointAtParameter(rec.t);
      rec.normal = (rec.p - center_) / radius_;
      return true;
    }
  }
  return false;
}

#endif