#ifndef WORLD_H
#define WORLD_H

#include <initializer_list>
#include <memory>
#include <vector>
#include "pointlight.h"
#include "sphere.h"

namespace scene {
class World {
   public:
    World() = default;

    inline std::shared_ptr<lighting::PointLight> light() const { return light_; }
    inline std::vector<std::shared_ptr<geometry::Shape>> objects() const { return objects_; }

    // factory fn for constructing what the book describes as the "Default World"
    static World DefaultWorld();

    void AddObject(std::shared_ptr<geometry::Shape> object_ptr);

    void AddObjects(std::initializer_list<std::shared_ptr<geometry::Shape>> object_ptrs);

    void AddObjects(std::vector<std::shared_ptr<geometry::Shape>>&& Shape_vec);

    void SetLight(std::shared_ptr<lighting::PointLight> light);

    bool WorldContains(const std::shared_ptr<geometry::Shape>& object) const;

    std::vector<geometry::Intersection> Intersect(const commontypes::Ray& ray) const;

    commontypes::Color ShadeHit(geometry::Computations& comps,
                                uint8_t remaining_invocations = RECURSION_LIMIT) const;

    commontypes::Color ColorAt(commontypes::Ray& r,
                               uint8_t remaining_invocations = RECURSION_LIMIT) const;

    commontypes::Color ReflectedColor(geometry::Computations& comps,
                                      u_int8_t remaining_invocations = RECURSION_LIMIT) const;

    commontypes::Color RefractedColor(geometry::Computations& comps,
                                      u_int8_t remaining_invocations = RECURSION_LIMIT);

    bool IsShadowed(const commontypes::Point& point) const;

   private:
    std::shared_ptr<lighting::PointLight> light_;
    std::vector<std::shared_ptr<geometry::Shape>> objects_;
    static const uint8_t RECURSION_LIMIT = 5;
};
}  // namespace scene

#endif  // WORLD_H
