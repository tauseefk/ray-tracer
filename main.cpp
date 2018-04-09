#include "camera.h"
#include "hitable_list.h"
#include "material.h"
#include "sphere.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

// XXX:TODO format is off
vec3 color(const ray& r, hitable* world, int depth) {
  hit_record rec;

  if (world->hit(r, 0.001, MAXFLOAT, rec)) {
    ray scattered;
    vec3 attenuation;
    if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      return attenuation * color(scattered, world, depth + 1);
    } else {
      return vec3(0, 0, 0);
    }
  } else {
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
  }
}

// XXX:TODO modify to pass const references instead of values.
void cal_img_part(const int p, const int parts, const int nx, const int ny,
                  const int ns, hitable* world, camera cam, float ray_v,
                  std::string* img_parts) {

  std::stringstream s;
  for (int j = ((p + 1) * (ny / parts)) - 1; j >= p * (ny / parts); j--) {
    for (int i = 0; i < nx; i++) {
      vec3 col(0, 0, 0);
      for (int s = 0; s < ns; s++) {
        float u = float(i + drand48()) / float(nx);
        float v = float(ray_v + drand48()) /
                  float(ny); // we can use j instead of ray_v in case of
                             // execution in a single chunk
        ray r = cam.get_ray(u, v);
        vec3 p = r.point_at_parameter(2.0);
        col += color(r, world, 0);
      }
      col /= float(ns);
      col = vec3(sqrt(col[0]), sqrt(col[1]),
                 sqrt(col[2])); // gamma correction; raise to 1/gamma;
      int ir = int(255.99 * col[0]);
      int ig = int(255.99 * col[1]);
      int ib = int(255.99 * col[2]);

      s << ir << " " << ig << " " << ib << std::endl;
    }
    if (ray_v > 0) {
      ray_v--;
    }
  }
  img_parts[p] = s.str();
}

int main() {
  int nx = 600;
  int ny = 300;
  int ns = 100;
  std::cout << "P3" << std::endl
            << nx << " " << ny << std::endl
            << "255" << std::endl;

  hitable* list[5];
  list[0] = new sphere(vec3(0, 0, -1), 0.5, new dielectric(1.5));
  list[1] = new sphere(vec3(1, 0, -1), 0.5,
                       new metal(vec3(0.8, 0.8, 0.8), float(0.0)));
  list[2] =
      new sphere(vec3(2, -0.02, -1), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
  list[3] =
      new sphere(vec3(-1, 0, 1), 0.5, new lambertian(vec3(0.8, 0.8, 0.2)));
  list[4] = new sphere(vec3(0, -100.5, -1), 100,
                       new metal(vec3(0.8, 0.6, 0.2), float(0.0)));
  hitable* world = new hitable_list(list, 5);

  vec3 lookfrom(0, 0, 2);
  vec3 lookat(0, 0, -1);
  camera cam(lookfrom, lookat, vec3(0, 1, 0), 60, float(nx) / float(ny),
             float(1.0 / 8.0), (lookfrom - lookat).length());

  // time_t start, end;
  // time(&start);

  const int parts = 3; // template argument can't reference unless
                       // it's a const length array

  std::string img_parts[parts];
  float ray_v = ny - 1; // we need the rays to be consistent despite breaking
                        // the work into several chunks

  std::vector<std::thread> th;

  // XXX:TODO create a thread pool and assign parts instead of deciding upfront
  for (int i = 0; i < parts; i++) { // can't spin the max number of threads coz
                                    // the main process is gonna use one
    th.push_back(std::thread(cal_img_part, i, parts, nx, ny, ns, world, cam,
                             ((i + 1) * ny / parts) - 1, std::ref(img_parts)));
  }

  for (auto& t : th) {
    t.join();
  }

  for (int i = 0; i < parts; i++) {
    std::cout << img_parts[parts - i - 1];
  }
  // time(&end);
  // std::cout << difftime(end, start) << " seconds" << std::endl;
}