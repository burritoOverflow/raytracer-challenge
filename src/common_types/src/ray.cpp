#include "ray.h"

commontypes::Ray operator*(const commontypes::Ray& r, const commontypes::Matrix& m) {
    assert(m.n_rows() == 4);
    const commontypes::Point origin = commontypes::Point{m * r.origin()};
    const commontypes::Vector direction = commontypes::Vector{m * r.direction()};
    return commontypes::Ray{origin, direction};
}

commontypes::Ray commontypes::Ray::Transform(const commontypes::Matrix& m) const {
    return (*this) * m;
}
