#ifndef SPHERE_H
#define SPHERE_H

#include <cstdint>
#include <vector>
#include "identitymatrix.h"
#include "intersection.h"
#include "material.h"
#include "point.h"
#include "ray.h"

namespace geometry {
class Sphere {
   public:
    // default Point ctor returns a Point with (0,0,0)
    Sphere()
        : origin_(commontypes::Point()),
          radii_(1.0),
          id_(SPHERE_ID++),
          transform_(commontypes::IdentityMatrix{}),
          material_(lighting::Material{}) {}

    // "collection of t values where Ray intersects the Sphere" -- we're using vec of Structs
    // containing the t val for an intersection and the id for the Sphere
    std::vector<Intersection> Intersect(const commontypes::Ray& ray);

    inline double radii() const { return radii_; }
    inline uint64_t id() const { return id_; }
    inline commontypes::Point origin() const { return origin_; }
    inline commontypes::Matrix transform() const { return transform_; }
    inline lighting::Material material() const { return material_; }

    inline void SetTransform(const commontypes::Matrix& transformation_matrix) {
        transform_ = transformation_matrix;
    }

    inline void SetMaterial(const lighting::Material& material) {
        material_ = std::move(material);
    }

    commontypes::Vector NormalAt(const commontypes::Point& p) const;

   private:
    uint64_t id_;   // each sphere must have a unique identifier
    double radii_;  // expectation is that by default these are all unit spheres (see page 59)
    commontypes::Point origin_;  // expectation is that the Sphere is situated at the World's
                                 // origin (0,0,0) (also 59)
    // each Sphere has a "default translation" (see page 69) here it's the IdentityMatrix
    commontypes::Matrix transform_;

    lighting::Material material_;  // each Sphere has a Material

    // must be incremented in each ctor, as above (see uniqueness constraint)
    static uint64_t SPHERE_ID;
};

}  // namespace geometry

bool operator==(const geometry::Sphere& s1, const geometry::Sphere& s2);

#endif  // SPHERE_H
