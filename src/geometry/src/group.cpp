#include "group.h"

// we want all Intersections ordered by ascending t values
static bool AscendingGeometryIntersectionComparator(const geometry::Intersection& intersection1,
                                                    const geometry::Intersection& intersection2) {
    return intersection1.t_ < intersection2.t_;
}

void geometry::Group::AddChildToGroup(std::shared_ptr<geometry::Shape>& shape_ptr) {
    shape_ptr->SetParent(this);
    this->children_.emplace_back(shape_ptr);
}

std::vector<geometry::Intersection> geometry::Group::LocalIntersect(
    const commontypes::Ray& ray) const {
    std::vector<geometry::Intersection> intersections{};

    // add the intersections for each Shape
    for (const auto& child : children_) {
        // TODO: perhaps insert these in sorted order?
        const auto child_intersections = child->Intersect(ray);
        intersections.insert(intersections.end(), child_intersections.begin(),
                             child_intersections.end());
    }

    // sort the collection of all Shapes' Intersections
    std::sort(intersections.begin(), intersections.end(), AscendingGeometryIntersectionComparator);
    return intersections;
}

commontypes::Vector geometry::Group::LocalNormalAt(const commontypes::Point& local_point) const {
    // placeholder; this should not be called.
    throw IncorrectCallException();
}

void geometry::Group::AddChildrenToGroup(std::initializer_list<std::shared_ptr<Shape>>& children) {
    for (auto child_ptr : children) {
        this->AddChildToGroup(child_ptr);
    }
}
