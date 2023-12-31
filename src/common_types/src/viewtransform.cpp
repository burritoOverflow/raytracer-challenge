#include "viewtransform.h"

commontypes::ViewTransform::ViewTransform(const commontypes::Point& from,
                                          const commontypes::Point& to,
                                          const commontypes::Vector& up)
    : Matrix(4, 4) {
    commontypes::Vector forward = commontypes::Vector{(to - from).Normalize()};
    commontypes::Vector left = forward.Cross(commontypes::Vector{up.Normalize()});
    commontypes::Vector true_up = left.Cross(forward);

    matrixtype orientation{};
    orientation.emplace_back(std::vector<double>{left.x(), left.y(), left.z(), 0});
    orientation.emplace_back(std::vector<double>{true_up.x(), true_up.y(), true_up.z(), 0});
    orientation.emplace_back(std::vector<double>{-forward.x(), -forward.y(), -forward.z(), 0});
    orientation.emplace_back(std::vector<double>{0, 0, 0, 1});

    const auto result = Matrix(orientation) * TranslationMatrix(-from.x(), -from.y(), -from.z());
    matrix_ = result.matrix();
}
