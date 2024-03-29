#ifndef CONE_H
#define CONE_H

#include "shape.h"

namespace geometry {
class Cone : public Shape {
   public:
    Cone()
        : Shape(),
          minimum_(-std::numeric_limits<double>::infinity()),
          maximum_(std::numeric_limits<double>::infinity()),
          capped_(false) {}

    explicit Cone(const double minimum, const double maximum, const bool capped)
        : minimum_(minimum), maximum_(maximum), capped_(capped) {}

    bool IsCapped() const { return capped_; }
    void SetIsCapped(const bool capped) { capped_ = capped; }

    std::vector<Intersection> LocalIntersect(const commontypes::Ray& ray) const override;

    commontypes::Vector LocalNormalAt(const commontypes::Point& local_point) const override;

   private:
    // used to constrain args for CheckCap, as only the min or max values are valid arguments
    enum PlaneYCoord { kUseMaximum, kUseMinimum };

    void IntersectCaps(const commontypes::Ray& ray, std::vector<geometry::Intersection>& xs) const;

    // radius is the y-coordinate of the Plane being tested, either the Cone's min or max.
    // this value is treated as the radius within the Point must lie
    // (see: pg. 190)
    bool CheckCap(const commontypes::Ray& ray, double t, PlaneYCoord plane_y_coord) const;

    inline bool IsYBetweenMinMax(const double y_val) const {
        return minimum_ < y_val && maximum_ > y_val;
    }

    // see the Cylinder implementation for details
    double minimum_;
    double maximum_;
    bool capped_;
};
}  // namespace geometry

#endif  // CONE_H
