#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "point.h"
#include "shape.h"

namespace geometry {
class Triangle : public Shape {
   public:
    explicit Triangle(const commontypes::Point& p1,
                      const commontypes::Point& p2,
                      const commontypes::Point& p3)
        : p1_(p1), p2_(p2), p3_(p3) {
        // precompute the normal vector; used for every point on the Triangle
        // these values remain everywhere on the Triangle (pg. 209)
        e1_ = commontypes::Vector{p2 - p1};
        e2_ = commontypes::Vector{p3 - p1};
        normal_ = commontypes::Vector{e2_.Cross(e1_).Normalize()};
    }

    inline const commontypes::Point& P1() const { return p1_; }
    void setP1(const commontypes::Point& p1) { p1_ = p1; }

    inline const commontypes::Point& P2() const { return p2_; }
    void setP2(const commontypes::Point& p2) { p2_ = p2; }

    inline const commontypes::Point& P3() const { return p3_; }
    void setP3(const commontypes::Point& p3) { p3_ = p3; }

    const commontypes::Vector& E1() const { return e1_; }

    const commontypes::Vector& E2() const { return e2_; }

    inline const commontypes::Vector& Normal() const { return normal_; }
    void setNormal(const commontypes::Vector& normal) { normal_ = normal; }

    std::vector<Intersection> LocalIntersect(const commontypes::Ray& ray) const override;

    commontypes::Vector LocalNormalAt(const commontypes::Point& local_point) const override;

   private:
    commontypes::Point p1_, p2_, p3_;  // each location of each corner in object space
    commontypes::Vector e1_, e2_;      // two edge Vectors
    commontypes::Vector normal_;
};
}  // namespace geometry

#endif  // TRIANGLE_H
