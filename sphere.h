#ifndef SPHEREH
#define SPHEREH
#include "hitable.h"

class sphere : public hitable {
public:
  sphere() {}
  sphere(vec3 cen, float r, material* m) : center(cen), radius(r), mat_ptr(m) {}
  virtual bool hit(const ray& r, float t_min, float t_max,
                   hit_record& rec) const;
  vec3 center;
  float radius;
  material* mat_ptr;
};

/**
 * Dot product of a ray from a point on the sphere to it's center would be the
 * square of the circle's radius dot.
 * ((A-C + t*B), (A-C + t*B)) - R*R = 0;
 * t*t*dot(B, B) + 2*t*dot((A-C), B) + dot ((A-C), (A-C)) - R*R = 0;
 */

bool sphere::hit(const ray& r, float t_min, float t_max,
                 hit_record& rec) const {
  vec3 oc = r.origin() - center;               // A - C;
  float a = dot(r.direction(), r.direction()); // dot(B, B);
  float b = dot(oc, r.direction());            // dot(A-C, B);
  float c = dot(oc, oc) - radius * radius;     // dot ((A-C), (A-C)) - R*R;
  float discriminant = b * b - (a * c);
  if (discriminant > 0) {
    float temp = (-b - sqrt(discriminant)) / (a);
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      rec.normal = (rec.p - center) / radius;
      rec.mat_ptr = mat_ptr;
      return true;
    }
    temp = (-b + sqrt(discriminant)) / a;
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      rec.normal = (rec.p - center) / radius;
      rec.mat_ptr = mat_ptr;
      return true;
    }
  }
  return false;
}
#endif