#ifndef _HITTABLE_H_
#define _HITTABLE_H_

#include "ray.h"

struct HitRecord
{
    float t;
    glm::vec3 p;
    glm::vec3 normal;
};

class Hittable
{
public:
    virtual bool hit(const Ray &r, float min, float max, HitRecord &rec) const = 0;
};

#endif