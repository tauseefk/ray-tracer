#include "ray.h"
#include "vec3.h"
#include <iostream>

/**
 * Dot product of a ray from a point on the sphere to it's center would be the
 * square of the circle's radius dot.
 * ((A-C + t*B), (A-C + t*B)) - R*R = 0;
 * t*t*dot(B, B) + 2*t*dot((A-C), B) + dot ((A-C), (A-C)) - R*R = 0;
 */
float hit_sphere(const ray& r, const vec3& center, float radius) {

  vec3 oc = r.origin() - center;               // A - C;
  float a = dot(r.direction(), r.direction()); // dot(B, B);
  float b = 2 * dot(oc, r.direction());        // dot(A-C, B);
  float c = dot(oc, oc) - radius * radius;     // dot ((A-C), (A-C)) - R*R;
  float discriminant = b * b - (4 * a * c);
  if (discriminant < 0) {
    return -1.0;
  } else {
    return (-b - sqrt(discriminant)) / (2.0 * a);
  }
}

vec3 color(ray& r) {
  float t = hit_sphere(r, vec3(0, 0, -1), 0.5);
  if (t > 0.0) {
    vec3 N = unit_vector(r.point_at_parameter(t) - vec3(0, 0, -1));
    return 0.5 * vec3(N.x() + 1, N.y() + 1, N.z() + 1);
  }
  vec3 unit_direction = unit_vector(r.direction());
  t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main() {
  int nx = 400;
  int ny = 200;
  std::cout << "P3" << std::endl
            << nx << " " << ny << std::endl
            << "255" << std::endl;

  vec3 lower_left_corner(-2.0, -1.0, -1.0);
  vec3 horizontal(4.0, 0.0, 0.0);
  vec3 vertical(0.0, 2.0, 0.0);
  vec3 origin(0.0, 0.0, 0.0);

  for (int j = ny - 1; j >= 0; j--) {
    for (int i = 0; i < nx; i++) {
      float u = float(i) / float(nx);
      float v = float(j) / float(ny);
      ray r(origin, lower_left_corner + u * horizontal + v * vertical);
      vec3 col = color(r);
      int ir = int(255.99 * col[0]);
      int ig = int(255.99 * col[1]);
      int ib = int(255.99 * col[2]);
      std::cout << ir << " " << ig << " " << ib << std::endl;
    }
  }
}