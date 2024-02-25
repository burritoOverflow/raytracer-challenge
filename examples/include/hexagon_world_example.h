#ifndef HEXAGON_WORLD_EXAMPLE_H
#define HEXAGON_WORLD_EXAMPLE_H

#include "cylinder.h"
#include "group.h"
#include "rotationmatrix.h"
#include "scalingmatrix.h"
#include "sphere.h"
#include "translationmatrix.h"

inline std::shared_ptr<geometry::Sphere> MakeHexagonCorner() {
    auto corner = std::make_shared<geometry::Sphere>();
    corner->SetTransform(commontypes::TranslationMatrix{0, 0, -1} *
                         commontypes::ScalingMatrix{0.25, 0.25, 0.25});
    return corner;
}

inline std::shared_ptr<geometry::Cylinder> MakeHexagonEdge() {
    auto edge = std::make_shared<geometry::Cylinder>();
    edge->SetMinimum(0);
    edge->SetMaximum(1);
    edge->SetTransform(
        commontypes::TranslationMatrix{0, 0, -1} * commontypes::RotationMatrixY(-M_PI / 6) *
        commontypes::RotationMatrixZ(-M_PI_2) * commontypes::ScalingMatrix{0.25, 1, 0.25});
    return edge;
}

inline std::shared_ptr<geometry::Group> MakeHexagonSide() {
    auto side = std::make_shared<geometry::Group>();

    std::shared_ptr<geometry::Shape> corner = MakeHexagonCorner();
    side->AddChildToGroup(corner);

    std::shared_ptr<geometry::Shape> edge = MakeHexagonEdge();
    side->AddChildToGroup(edge);

    return side;
}

inline std::shared_ptr<geometry::Group> MakeHexagon() {
    auto hexagon = std::make_shared<geometry::Group>();
    for (size_t i = 0; i < 6; ++i) {
        std::shared_ptr<geometry::Shape> side = MakeHexagonSide();
        side->SetTransform(commontypes::RotationMatrixY{i * M_PI / 3});
        hexagon->AddChildToGroup(side);
    }
    return hexagon;
}

#endif  // HEXAGON_WORLD_EXAMPLE_H
