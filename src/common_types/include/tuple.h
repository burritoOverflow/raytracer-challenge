#ifndef TUPLE_H
#define TUPLE_H

#include <cmath>
#include <limits>
#include "utility.h"

namespace commontypes {
class Tuple {
   public:
    Tuple() {
        for (double& i : e_) {
            i = 0.0;
        }
    }

    Tuple(const double x, const double y, const double z, const double w) {
        e_[0] = x;
        e_[1] = y;
        e_[2] = z;
        e_[3] = w;
    }

    inline double x() const { return e_[0]; }
    inline double y() const { return e_[1]; }
    inline double z() const { return e_[2]; }
    inline double w() const { return e_[3]; }

    // w == 1.0 is a Point
    inline bool IsPoint() const { return fabs(w() - 1.0) < utility::EPSILON_; }

    // w == 0 is a Vector
    inline bool IsVector() const { return fabs(w()) < utility::EPSILON_; }

    inline double Magnitude() const {
        return sqrt(e_[0] * e_[0] + e_[1] * e_[1] + e_[2] * e_[2] + e_[3] * e_[3]);
    };

    inline Tuple Normalize() const {
        const double magnitude = this->Magnitude();
        return {e_[0] / magnitude, e_[1] / magnitude, e_[2] / magnitude, e_[3] / magnitude};
    }

    inline double Dot(const Tuple& t) const {
        return this->x() * t.x() + this->y() * t.y() + this->z() * t.z() + this->w() + t.w();
    }

    Tuple Reflect(const Tuple& normal) const;

    inline Tuple operator-() {
        return Tuple{
            -e_[0],
            -e_[1],
            -e_[2],
            -e_[3],
        };
    }

    inline double operator[](size_t i) const { return e_[i]; }
    inline double& operator[](size_t i) { return e_[i]; }

    Tuple& operator+=(const Tuple& t);
    Tuple& operator-=(const Tuple& t);

    double e_[4]{};  // individual tuple elements
};
}  // namespace commontypes

commontypes::Tuple operator+(const commontypes::Tuple& t1, const commontypes::Tuple& t2);
commontypes::Tuple operator-(const commontypes::Tuple& t1, const commontypes::Tuple& t2);
commontypes::Tuple operator*(const commontypes::Tuple& t, double d);
commontypes::Tuple operator/(const commontypes::Tuple& t, double d);

bool operator==(const commontypes::Tuple& t1, const commontypes::Tuple& t2);
bool operator!=(const commontypes::Tuple& t1, const commontypes::Tuple& t2);

#endif  // TUPLE_H
