#ifndef TUPLE_H
#define TUPLE_H

#include <cmath>
#include <limits>

namespace commontypes {
class Tuple {
   public:
    Tuple() {
        for (size_t i = 0; i < 4; i++) {
            e_[i] = 0;
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
    bool IsPoint() const { return fabs(w() - 1.0) < EPSILON_; }

    // w == 0 is a Vector
    bool IsVector() const { return fabs(w()) < EPSILON_; }

    inline double Magnitude() {
        return sqrt(e_[0] * e_[0] + e_[1] * e_[1] + e_[2] * e_[2] + e_[3] * e_[3]);
    };

    inline Tuple Normalize() {
        const double magnitude = this->Magnitude();
        return {e_[0] / magnitude, e_[1] / magnitude, e_[2] / magnitude, e_[3] / magnitude};
    }

    inline double Dot(const Tuple& t) const {
        return this->x() * t.x() + this->y() * t.y() + this->z() * t.z() + this->w() + t.w();
    }

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

    static inline bool NearEquals(const double a, const double b) {
        return (fabs(a - b) <= EPSILON_ || a == b);
    }

   protected:
    double e_[4]{};  // individual tuple elements

   private:
    static constexpr double EPSILON_ = 0.00001;
};
}  // namespace commontypes

commontypes::Tuple operator+(commontypes::Tuple& t1, commontypes::Tuple& t2);
commontypes::Tuple operator-(commontypes::Tuple& t1, commontypes::Tuple& t2);
commontypes::Tuple operator*(commontypes::Tuple& t, double d);
commontypes::Tuple operator/(commontypes::Tuple& t, double d);

bool operator==(const commontypes::Tuple& t1, const commontypes::Tuple& t2);
bool operator!=(const commontypes::Tuple& t1, const commontypes::Tuple& t2);

#endif  // TUPLE_H