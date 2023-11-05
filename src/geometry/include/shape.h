#ifndef SHAPE_H
#define SHAPE_H

#include <memory>
#include "identitymatrix.h"
#include "intersection.h"
#include "lighting.h"
#include "matrix.h"
#include "point.h"

namespace geometry {
class Shape {
   public:
    Shape()
        : id_(SHAPE_ID++),
          transform_(commontypes::IdentityMatrix{}),
          material_(std::make_shared<lighting::Material>()) {}

    inline uint64_t id() const { return id_; }
    inline commontypes::Matrix transform() const { return transform_; }
    inline std::shared_ptr<lighting::Material> material() const { return material_; }

    inline void SetTransform(const commontypes::Matrix& transformation_matrix) {
        transform_ = transformation_matrix;
    }

    inline void SetMaterial(const std::shared_ptr<lighting::Material>& material) {
        material_ = material;
    }

    std::vector<Intersection> Intersect(const commontypes::Ray& ray);

   protected:
    commontypes::Matrix transform_;  // each Shape has a transformation matrix (see page 118); here
                                     // it's the IdentityMatrix
    std::shared_ptr<lighting::Material> material_;  // each Sphere has a Material

    // each shape provides its own appropriate implementation
    virtual std::vector<Intersection> LocalIntersect(const commontypes::Ray& ray) = 0;

   private:
    static uint64_t SHAPE_ID;  // each shape must have a unique identifier
    uint64_t id_;              // this shape's identifier
};

class TestShape : public Shape {
   public:
    TestShape() : Shape(), saved_ray_{commontypes::Point{}, commontypes::Vector{}} {};

    std::vector<Intersection> LocalIntersect(const commontypes::Ray& ray) override {
        saved_ray_ = ray;
        return {};
    }

    // see pg. 120
    commontypes::Ray saved_ray_;
};
}  // namespace geometry
#endif  // SHAPE_H