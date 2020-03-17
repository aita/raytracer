#include "config.h"
#include <limits.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "ray.h"
#include "sphere.h"
#include "hittable_list.h"

glm::vec3 Color(const Ray &r, Hittable &world)
{
  HitRecord rec;
  if (world.hit(r, 0.f, FLT_MAX, rec))
  {
    return .5f * glm::vec3(rec.normal.x + 1.f, rec.normal.y + 1.f, rec.normal.z + 1.f);
  }
  else
  {
    glm::vec3 unitDirection = glm::normalize(r.direction());
    float t = 0.5f * (unitDirection.y + 1.0f);
    return (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
  }
}

int main(int argc, char **argv)
{
  const int nx = 200;
  const int ny = 100;
  const int nc = 3;

  const glm::vec3 lowerLeftCorner(-2.0f, -1.0f, -1.0f);
  const glm::vec3 horizontal(4.0f, 0.0f, 0.0f);
  const glm::vec3 vertical(0.0f, 2.0f, 0.0f);
  const glm::vec3 origin(0.0f, 0.0f, 0.0f);

  HittableList world = {
      std::make_shared<Sphere>(glm::vec3(0.f, 0.f, -1.f), .5f),
      std::make_shared<Sphere>(glm::vec3(0.f, -100.5f, -1.f), 100.f),
  };

  std::vector<uint8_t>
      pixels(nx * ny * nc);
  size_t index = 0;
  for (int j = ny - 1; j >= 0; --j)
  {
    for (int i = 0; i < nx; ++i)
    {
      auto u = float(i) / float(nx);
      auto v = float(j) / float(ny);
      Ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);
      auto col = Color(r, world);
      pixels[index++] = uint8_t(255.99f * col[0]);
      pixels[index++] = uint8_t(255.99f * col[1]);
      pixels[index++] = uint8_t(255.99f * col[2]);
    }
  }

  if (stbi_write_png("schene.png", nx, ny, nc, &pixels[0], nx * nc) == 0)
  {
    std::cerr << "failed to write an image." << std::endl;
    return 1;
  }

  return 0;
}