#ifndef GROUP_H
#define GROUP_H

#include "shape.h"

// Group allows for collecting several Shapes as a single unit, where the Group acts as the parent
// for the Shapes that comprise the Group
namespace geometry {
class Group : public Shape {
   public:
    Group() : Shape() {}

    const std::vector<std::shared_ptr<Shape>> GetChildren() const { return this->children_; }

    void AddChildToGroup(std::shared_ptr<Shape>& shape_ptr);

    void AddChildrenToGroup(std::initializer_list<std::shared_ptr<Shape>>& children);

    std::vector<Intersection> LocalIntersect(const commontypes::Ray& ray) const override;

    commontypes::Vector LocalNormalAt(const commontypes::Point& local_point) const override;

   private:
    std::vector<std::shared_ptr<Shape>> children_;
};
}  // namespace geometry

// we'll throw as per the suggestion on pg. 200
// LocalNormalAt should not be called on a Group; we'll throw with this message in that case.
class IncorrectCallException : public std::logic_error {
   public:
    IncorrectCallException()
        : std::logic_error("`LocalNormalAt` should not be called on a Group!"){};
};

#endif  // GROUP_H
