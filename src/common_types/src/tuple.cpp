#include "tuple.h"

using namespace commontypes;

Tuple operator+(Tuple& t1, Tuple& t2) {
    return {t1.x() + t2.x(), t1.y() + t2.y(), t1.z() + t2.z(), t1.w() + t2.w()};
}

Tuple operator-(Tuple& t1, Tuple& t2) {
    return {t1.x() - t2.x(), t1.y() - t2.y(), t1.z() - t2.z(), t1.w() - t2.w()};
}

Tuple operator*(Tuple& t, double d) {
    return {t.x() * d, t.y() * d, t.z() * d, t.w() * d};
}

Tuple operator/(Tuple& t, double d) {
    return {t.x() / d, t.y() / d, t.z() / d, t.w() / d};
}

bool operator==(const Tuple& t1, const Tuple& t2) {
    const double t1_elements[] = {t1.x(), t1.y(), t1.z(), t1.w()};
    const double t2_elements[] = {t2.x(), t2.y(), t2.z(), t2.w()};

    for (size_t i = 0; i < 4; i++) {
        // perform a "double equals" for each element in each Tuple
        if (!Tuple::NearEquals(t1_elements[i], t2_elements[i])) {
            return false;
        }
    }

    return true;
}

Tuple& Tuple::operator+=(const Tuple& t) {
    e_[0] += t.e_[0];
    e_[1] += t.e_[1];
    e_[2] += t.e_[2];
    e_[3] += t.e_[3];

    return *this;
}

Tuple& Tuple::operator-=(const Tuple& t) {
    e_[0] -= t.e_[0];
    e_[1] -= t.e_[1];
    e_[2] -= t.e_[2];
    e_[3] -= t.e_[3];

    return *this;
}
