#ifndef SHAPE_H
#define SHAPE_H

#include <memory>
#include "identitymatrix.h"
#include "intersection.h"
#include "material.h"
#include "matrix.h"
#include "point.h"

namespace geometry {
class Shape {
   public:
    Shape()
        : id_(SHAPE_ID++),
          transform_(commontypes::IdentityMatrix{}),
          material_ptr_(std::make_shared<lighting::Material>()) {}

    explicit Shape(commontypes::Matrix& transformation_matrix,
                   std::shared_ptr<lighting::Material>& material_ptr)
        : id_(SHAPE_ID++), transform_(transformation_matrix), material_ptr_(material_ptr) {}

    inline uint64_t id() const { return id_; }
    inline commontypes::Matrix Transform() const { return transform_; }
    inline std::shared_ptr<lighting::Material> Material() const { return material_ptr_; }

    inline void SetTransform(const commontypes::Matrix& transformation_matrix) {
        transform_ = transformation_matrix;
    }

    inline commontypes::Matrix GetTransform() const { return transform_; }

    inline void SetMaterial(const std::shared_ptr<lighting::Material>& material) {
        material_ptr_ = material;
    }

    // when intersecting the shape with a Ray, all shapes need to first convert the Ray into
    // object space, transforming it by the inverse of the shape's transformation Matrix
    std::vector<Intersection> Intersect(const commontypes::Ray& ray) const;

    // responsible for transforming the point, invokes the shape-implemented `LocalNormalAt`
    // fn, transforms and returns the resulting normal
    commontypes::Vector NormalAt(const commontypes::Point& point) const;

   protected:
    commontypes::Matrix transform_;  // each Shape has a transformation matrix (see page 118); here
                                     // it's the IdentityMatrix
    std::shared_ptr<lighting::Material>
        material_ptr_;  // each Shape has a Material (the default one (see pg. 118 & 83)

    // each shape provides its own appropriate implementation for both local intersection and
    // local normal calculation
    virtual std::vector<Intersection> LocalIntersect(const commontypes::Ray& ray) const = 0;

    virtual commontypes::Vector LocalNormalAt(const commontypes::Point& local_point) const = 0;

   private:
    static uint64_t SHAPE_ID;  // each shape must have a unique identifier
    uint64_t id_;              // this shape's identifier
};
}  // namespace geometry

bool operator==(const geometry::Shape& s1, const geometry::Shape& s2);

#endif  // SHAPE_H