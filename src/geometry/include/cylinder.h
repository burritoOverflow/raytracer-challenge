#ifndef CYLINDER_H
#define CYLINDER_H

#include "intersection.h"
#include "shape.h"

namespace geometry {
class Cylinder : public Shape {
   public:
    Cylinder()
        : Shape(),
          minimum_(-std::numeric_limits<double>::infinity()),
          maximum_(std::numeric_limits<double>::infinity()),
          capped_(false) {}

    explicit Cylinder(const double minimum, const double maximum, const bool capped)
        : minimum_(minimum), maximum_(maximum), capped_(capped) {}

    inline const double Minimum() const { return minimum_; }
    void SetMinimum(const double minimum) { minimum_ = minimum; }

    inline const double Maximum() const { return maximum_; }
    void SetMaximum(const double maximum) { maximum_ = maximum; }

    bool IsCapped() const { return capped_; }
    void SetIsCapped(const bool capped) { capped_ = capped; }

    std::vector<Intersection> LocalIntersect(const commontypes::Ray& ray) override;

    commontypes::Vector LocalNormalAt(const commontypes::Point& local_point) override;

    static bool CheckCap(const commontypes::Ray& ray, const double t);

    void IntersectCaps(const commontypes::Ray& ray, std::vector<geometry::Intersection>& xs);

   private:
    // min and max are units on the y-axis and defined in object space
    // these values are exclusive; i.e does not include these limits (see pg. 182)
    double minimum_;
    double maximum_;
    bool capped_;  // is cylinder closed?
};
}  // namespace geometry

#endif  // CYLINDER_H
