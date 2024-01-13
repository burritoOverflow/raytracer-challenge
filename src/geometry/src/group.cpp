#include "group.h"

// we want all Intersections ordered by ascending t values
static bool AscendingGeometryIntersectionComparator(const geometry::Intersection& intersection1,
                                                    const geometry::Intersection& intersection2) {
    return intersection1.t_ < intersection2.t_;
}

void geometry::Group::AddChildToGroup(std::shared_ptr<geometry::Shape>& shape_ptr) {
    // TODO: not sure about the bidirectional shared_ptr between parent <-> child here; this should
    // be carefully considered
    std::shared_ptr<geometry::Shape> this_ptr = std::make_shared<geometry::Group>(*this);
    shape_ptr->SetParent(this_ptr);
    this->children_.emplace_back(shape_ptr);
}

std::vector<geometry::Intersection> geometry::Group::LocalIntersect(
    const commontypes::Ray& ray) const {
    std::vector<geometry::Intersection> intersections{};

    // add the intersections for each Shape
    for (const auto& child : children_) {
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
