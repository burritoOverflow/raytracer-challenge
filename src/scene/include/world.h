#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <vector>

#include "pointlight.h"
#include "sphere.h"

namespace scene {
class World {
   public:
    World() = default;

    inline std::shared_ptr<lighting::PointLight> light() const { return light_; }
    inline std::vector<std::shared_ptr<geometry::Sphere>> objects() const { return objects_; }

    // factory fn for constructing what the book describes as the "Default World"
    static World DefaultWorld();

    void AddObject(std::shared_ptr<geometry::Sphere> object);
    void SetLight(std::shared_ptr<lighting::PointLight> light);

    bool WorldContains(const std::shared_ptr<geometry::Sphere>& object) const;

    std::vector<geometry::Intersection> Intersect(const commontypes::Ray& ray);

   private:
    std::shared_ptr<lighting::PointLight> light_;
    std::vector<std::shared_ptr<geometry::Sphere>> objects_;
};
}  // namespace scene

#endif  // WORLD_H
