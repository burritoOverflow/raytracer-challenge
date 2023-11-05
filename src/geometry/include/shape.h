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

    // when intersecting the shape with a Ray, all shapes need to first convert the Ray into
    // object space, transforming it by the inverse of the shape's transformation Matrix
    std::vector<Intersection> Intersect(const commontypes::Ray& ray);

    // responsible for transforming the point, invokes the shape-implemented `LocalNormalAt`
    // fn, transforms and returns the resulting normal
    commontypes::Vector NormalAt(const commontypes::Point& point);

   protected:
    commontypes::Matrix transform_;  // each Shape has a transformation matrix (see page 118); here
                                     // it's the IdentityMatrix
    std::shared_ptr<lighting::Material>
        material_;  // each Shape has a Material (the default one (see pg. 118 & 83)

    // each shape provides its own appropriate implementation for both local intersection and
    // local normal calculation
    virtual std::vector<Intersection> LocalIntersect(const commontypes::Ray& ray) = 0;

    virtual commontypes::Vector LocalNormalAt(const commontypes::Point& local_point) = 0;

   private:
    static uint64_t SHAPE_ID;  // each shape must have a unique identifier
    uint64_t id_;              // this shape's identifier
};
}  // namespace geometry
#endif  // SHAPE_H