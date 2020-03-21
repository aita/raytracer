#include <Eigen/Dense>
#include <cfloat>
#include <cmath>
#include <iostream>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "camera.h"
#include "config.h"
#include "dielectric.h"
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
      return Eigen::Vector3f::Zero();
    }
  } else {
    Eigen::Vector3f unit_direction = r.direction().normalized();
    float t = 0.5f * (unit_direction.y() + 1.0f);
    return (1.0f - t) * Eigen::Vector3f(1.0f, 1.0f, 1.0f) +
           t * Eigen::Vector3f(0.5f, 0.7f, 1.0f);
  }
}

std::unique_ptr<Hittable> RandomScene() {
  std::vector<std::unique_ptr<Hittable>> v;
  v.emplace_back(std::make_unique<Sphere>(
      Eigen::Vector3f(0.f, -1000.f, 0.f), 1000.f,
      std::make_unique<Lambertian>(Eigen::Vector3f(.5f, .5f, .5f))));
  for (int a = -11; a < 11; ++a) {
    for (int b = -11; b < 11; ++b) {
      float choose_material = Random<double>();
      Eigen::Vector3f center(a + .9f * Random<double>(), .2f,
                             b + .9f + Random<double>());
      if ((center - Eigen::Vector3f(4.f, .2f, 0.f)).norm() > .9f) {
        if (choose_material < .8f)  // diffuse
        {
          v.emplace_back(std::make_unique<Sphere>(
              center, 0.2f,
              std::make_unique<Lambertian>(
                  Eigen::Vector3f(Random<double>() * Random<double>(),
                                  Random<double>() * Random<double>(),
                                  Random<double>() * Random<double>()))));

        } else if (choose_material < .95f)  // metal
        {
          v.emplace_back(std::make_unique<Sphere>(
              center, 0.2f,
              std::make_unique<Metal>(
                  Eigen::Vector3f(.5f * (1.f + Random<double>()),
                                  .5f * (1.f + Random<double>()),
                                  .5f * (1.f + Random<double>())),
                  .5f * Random<double>())));
        } else {  // glass
          v.emplace_back(std::make_unique<Sphere>(
              center, 0.2f, std::make_unique<Dielectric>(1.5f)));
        }
      }
    }
  }
  v.emplace_back(std::make_unique<Sphere>(Eigen::Vector3f(0.f, 1.f, 0.f), 1.f,
                                          std::make_unique<Dielectric>(1.5f)));
  v.emplace_back(std::make_unique<Sphere>(
      Eigen::Vector3f(-4.f, 1.f, 0.f), 1.f,
      std::make_unique<Lambertian>(Eigen::Vector3f(.4f, .2f, .1f))));
  v.emplace_back(std::make_unique<Sphere>(
      Eigen::Vector3f(4.f, 1.f, 0.f), 1.f,
      std::make_unique<Metal>(Eigen::Vector3f(.7f, .6f, .5f), 0.f)));
  return std::make_unique<HittableList>(std::move(v));
}

int main(int argc, char** argv) {
  const int nx = 1200;
  const int ny = 800;
  const int ns = 10;
  const int nc = 3;

  auto world = RandomScene();
  auto look_from = Eigen::Vector3f(13.f, 2.f, 3.f);
  auto look_at = Eigen::Vector3f(0.f, 0.f, 0.f);
  float distance_to_focus = 10.f;
  float aperture = .1f;
  Camera camera(look_from, look_at, Eigen::Vector3f(0.f, 1.f, 0.f), 20.f,
                float(nx) / float(ny), aperture, distance_to_focus);

  std::vector<uint8_t> pixels(nx * ny * nc);
#pragma omp parallel for collapse(2)
  for (int j = ny - 1; j >= 0; --j) {
    for (int i = 0; i < nx; ++i) {
      int k = i + (ny - 1 - j) * nx;
      Eigen::Vector3f color(0.f, 0.f, 0.f);
      for (int s = 0; s < ns; ++s) {
        auto u = float(i + Random<double>()) / float(nx);
        auto v = float(j + Random<double>()) / float(ny);
        auto ray = camera.getRay(u, v);
        color += Color(ray, *world, 0);
      }
      color /= float(ns);
      color = color.cwiseSqrt();

      pixels[k * 3] = uint8_t(255.99f * color[0]);
      pixels[k * 3 + 1] = uint8_t(255.99f * color[1]);
      pixels[k * 3 + 2] = uint8_t(255.99f * color[2]);
    }
  }

  if (stbi_write_png("schene.png", nx, ny, nc, &pixels[0], nx * nc) == 0) {
    std::cerr << "failed to write an image." << std::endl;
    return 1;
  }

  return 0;
}