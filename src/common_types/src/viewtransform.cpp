#include "viewtransform.h"

commontypes::ViewTransform::ViewTransform(commontypes::Point& from,
                                          commontypes::Point& to,
                                          commontypes::Vector& up)
    : Matrix(4, 4) {
    commontypes::Vector forward = (to - from).Normalize();
    commontypes::Vector left = forward.Cross(up.Normalize());
    commontypes::Vector true_up = left.Cross(forward);

    matrixtype orientation;
    orientation.emplace_back(std::vector<double>{left.x(), left.y(), left.z(), 0});
    orientation.emplace_back(std::vector<double>{true_up.x(), true_up.y(), true_up.z(), 0});
    orientation.emplace_back(std::vector<double>{-forward.x(), -forward.y(), -forward.z(), 0});
    orientation.emplace_back(std::vector<double>{0, 0, 0, 1});

    auto result = Matrix(orientation) * TranslationMatrix(-from.x(), -from.y(), -from.z());
    matrix_ = result.matrix();
}
