#ifndef _SPHERE_H_
#define _SPHERE_H_
#include <cmath>
#include <glm/glm.hpp>
#include "hittable.h"

class Sphere : public Hittable
{
    glm::vec3 center_;
    float radius_;

public:
    Sphere(glm::vec3 center, float r) : center_(center), radius_(r) {}
    virtual bool hit(const Ray &r, float tmin, float tmax, HitRecord &rec) const;
};

bool Sphere::hit(const Ray &r, float tmin, float tmax, HitRecord &rec) const
{
    glm::vec3 oc = r.origin() - center_;
    float a = glm::dot(r.direction(), r.direction());
    float b = glm::dot(oc, r.direction());
    float c = glm::dot(oc, oc) - radius_ * radius_;
    float discriminant = b * b - a * c;
    if (discriminant > 0)
    {
        float temp = (-b - std::sqrt(discriminant)) / a;
        if (temp < tmax && temp > tmin)
        {
            rec.t = temp;
            rec.p = r.pointAtParameter(rec.t);
            rec.normal = (rec.p - center_) / radius_;
            return true;
        }
        temp = (-b + std::sqrt(discriminant)) / a;
        if (temp < tmax && temp > tmin)
        {
            rec.t = temp;
            rec.p = r.pointAtParameter(rec.t);
            rec.normal = (rec.p - center_) / radius_;
            return true;
        }
    }
    return false;
}

#endif