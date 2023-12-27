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
          maximum_(std::numeric_limits<double>::infinity()) {}

    std::vector<Intersection> LocalIntersect(const commontypes::Ray& ray) override;

    commontypes::Vector LocalNormalAt(const commontypes::Point& local_point) override;

    inline const double Minimum() const { return minimum_; }
    void SetMinimum(double minimum) { minimum_ = minimum; }

    inline const double Maximum() const { return maximum_; }
    void SetMaximum(double maximum) { maximum_ = maximum; }

   private:
    double minimum_;
    double maximum_;
};
}  // namespace geometry

#endif  // CYLINDER_H
