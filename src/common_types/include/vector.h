#ifndef VECTOR_H
#define VECTOR_H

#include "tuple.h"

namespace commontypes {
class Vector final : public Tuple {
   public:
    Vector() : Tuple() {}
    Vector(const double x, const double y, const double z) : Tuple(x, y, z, 0.0) {}
    Vector(const Tuple& t) : Tuple(t.x(), t.y(), t.z(), 0.0) {}

    inline Vector Cross(const Vector& v) {
        return {this->y() * v.z() - this->z() * v.y(), this->z() * v.x() - this->x() * v.z(),
                this->x() * v.y() - this->y() * v.x()};
    }
};

}  // namespace commontypes

#endif  // VECTOR_H
