#include <Eigen/Dense>
#include <cfloat>
#include <cmath>
#include <iostream>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "camera.h"
#include "config.h"
#include "hittable_list.h"
#include "lambertian.h"
#include "metal.h"
#include "random.h"
#include "ray.h"
#include "sphere.h"

Eigen::Vector3f Color(const Ray& r, Hittable& world, int depth) {
  HitRecord rec;
  if (world.hit(r, 0.001f, FLT_MAX, rec)) {
    Ray scattered;
    Eigen::Vector3f attenuation;
    if (depth < 50 &&
        rec.material->get().scatter(r, rec, attenuation, scattered)) {
      return attenuation.cwiseProduct(Color(scattered, world, depth + 1));
    } else {
      return Eigen::Vector3f(0.f, 0.f, 0.f);
    }
  } else {
    Eigen::Vector3f unit_direction = r.direction().normalized();
    float t = 0.5f * (unit_direction.y() + 1.0f);
    return (1.0f - t) * Eigen::Vector3f(1.0f, 1.0f, 1.0f) +
           t * Eigen::Vector3f(0.5f, 0.7f, 1.0f);
  }
}

int main(int argc, char** argv) {
  const int nx = 200;
  const int ny = 100;
  const int ns = 100;
  const int nc = 3;

  HittableList world = {
      std::make_shared<Sphere>(
          Eigen::Vector3f(0.f, 0.f, -1.f), .5f,
          std::make_shared<Lambertian>(Eigen::Vector3f(0.8f, 0.3f, 0.3f))),
      std::make_shared<Sphere>(
          Eigen::Vector3f(0.f, -100.5f, -1.f), 100.f,
          std::make_shared<Lambertian>(Eigen::Vector3f(0.8f, 0.8f, 0.0f))),
      std::make_shared<Sphere>(
          Eigen::Vector3f(1.f, 0.f, -1.f), .5f,
          std::make_shared<Metal>(Eigen::Vector3f(0.8f, 0.6f, 0.2f), 0.3f)),
      std::make_shared<Sphere>(
          Eigen::Vector3f(-1.f, 0.f, -1.f), .5f,
          std::make_shared<Metal>(Eigen::Vector3f(0.8f, 0.8f, 0.8f), 1.0f)),

  };
  Camera camera;

  std::vector<uint8_t> pixels(nx * ny * nc);
  size_t index = 0;
  for (int j = ny - 1; j >= 0; --j) {
    for (int i = 0; i < nx; ++i) {
      Eigen::Vector3f color(0.f, 0.f, 0.f);
      for (int s = 0; s < ns; ++s) {
        auto u = float(i + Random<double>()) / float(nx);
        auto v = float(j + Random<double>()) / float(ny);
        auto ray = camera.getRay(u, v);
        color += Color(ray, world, 0);
      }
      color /= float(ns);
      color = color.cwiseSqrt();

      pixels[index++] = uint8_t(255.99f * color[0]);
      pixels[index++] = uint8_t(255.99f * color[1]);
      pixels[index++] = uint8_t(255.99f * color[2]);
    }
  }

  if (stbi_write_png("schene.png", nx, ny, nc, &pixels[0], nx * nc) == 0) {
    std::cerr << "failed to write an image." << std::endl;
    return 1;
  }

  return 0;
}