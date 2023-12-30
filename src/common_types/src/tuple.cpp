#include "tuple.h"

commontypes::Tuple operator+(const commontypes::Tuple& t1, const commontypes::Tuple& t2) {
    return commontypes::Tuple{t1.x() + t2.x(), t1.y() + t2.y(), t1.z() + t2.z(), t1.w() + t2.w()};
}

commontypes::Tuple operator-(const commontypes::Tuple& t1, const commontypes::Tuple& t2) {
    return commontypes::Tuple{t1.x() - t2.x(), t1.y() - t2.y(), t1.z() - t2.z(), t1.w() - t2.w()};
}

commontypes::Tuple operator*(const commontypes::Tuple& t, const double d) {
    return commontypes::Tuple{t.x() * d, t.y() * d, t.z() * d, t.w() * d};
}

commontypes::Tuple operator/(const commontypes::Tuple& t, const double d) {
    return commontypes::Tuple{t.x() / d, t.y() / d, t.z() / d, t.w() / d};
}

bool operator==(const commontypes::Tuple& t1, const commontypes::Tuple& t2) {
    const double t1_elements[] = {t1.x(), t1.y(), t1.z(), t1.w()};
    const double t2_elements[] = {t2.x(), t2.y(), t2.z(), t2.w()};

    for (size_t i = 0; i < 4; i++) {
        // perform a "double equals" for each element in each Tuple
        if (!utility::NearEquals(t1_elements[i], t2_elements[i])) {
            return false;
        }
    }
    return true;
}

bool operator!=(const commontypes::Tuple& t1, const commontypes::Tuple& t2) {
    return !(t1 == t2);
}

commontypes::Tuple& commontypes::Tuple::operator+=(const commontypes::Tuple& t) {
    e_[0] += t.e_[0];
    e_[1] += t.e_[1];
    e_[2] += t.e_[2];
    e_[3] += t.e_[3];

    return *this;
}

commontypes::Tuple& commontypes::Tuple::operator-=(const commontypes::Tuple& t) {
    e_[0] -= t.e_[0];
    e_[1] -= t.e_[1];
    e_[2] -= t.e_[2];
    e_[3] -= t.e_[3];

    return *this;
}

commontypes::Tuple commontypes::Tuple::Reflect(const commontypes::Tuple& normal) const {
    return *this - normal * 2.0 * this->Dot(normal);
}
