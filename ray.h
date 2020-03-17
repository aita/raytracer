#ifndef _RAY_H_
#define _RAY_H_
#include <glm/vec3.hpp>

class Ray
{
    glm::vec3 Origin;
    glm::vec3 Direction;

public:
    Ray() {}
    Ray(const glm::vec3 &a, const glm::vec3 &b) : Origin(a), Direction(b) {}
    glm::vec3 origin() const { return Origin; }
    glm::vec3 direction() const { return Direction; }
    glm::vec3 pointAtParameter(float t) const { return Origin + t * Direction; }
};

#endif