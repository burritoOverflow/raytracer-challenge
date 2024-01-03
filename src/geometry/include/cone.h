#ifndef CONE_H
#define CONE_H

#include "shape.h"

namespace geometry {
class Cone : public Shape {
   public:
    Cone()
        : minimum_(-std::numeric_limits<double>::infinity()),
          maximum_(std::numeric_limits<double>::infinity()),
          capped_(false) {}

    explicit Cone(const double minimum, const double maximum, const bool capped)
        : minimum_(minimum), maximum_(maximum), capped_(capped) {}

    bool IsCapped() const { return capped_; }
    void SetIsCapped(const bool capped) { capped_ = capped; }

    std::vector<Intersection> LocalIntersect(const commontypes::Ray& ray) override;

    commontypes::Vector LocalNormalAt(const commontypes::Point& local_point) override;

    // radius is the y-coordinate of the Plane being tested (see: pg. 190)
    static bool CheckCap(const commontypes::Ray& ray, const double t, const double radius);

    void IntersectCaps(const commontypes::Ray& ray, std::vector<geometry::Intersection>& xs);

   private:
    // see the Cylinder implementation for details
    double minimum_;
    double maximum_;
    bool capped_;
};
}  // namespace geometry

#endif  // CONE_H
