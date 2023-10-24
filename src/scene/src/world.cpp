#include "world.h"
#include <algorithm>
#include <utility>
#include "lighting.h"
#include "scalingmatrix.h"

using sphereptr = std::shared_ptr<geometry::Sphere>;

// see description of the "Default World" on pg. 92
scene::World scene::World::DefaultWorld() {
    scene::World world{};
    lighting::PointLight light{commontypes::Point{-10, 10, -10}, commontypes::Color{1, 1, 1}};
    world.SetLight(std::make_shared<lighting::PointLight>(light));

    lighting::Material material{};
    material.SetColor(commontypes::Color{0.8, 1.0, 0.6});
    material.SetDiffuse(0.7);
    material.SetSpecular(0.2);

    geometry::Sphere s1{};
    s1.SetMaterial(std::make_shared<lighting::Material>(std::move(material)));

    geometry::Sphere s2{};
    s2.SetTransform(commontypes::ScalingMatrix{0.5, 0.5, 0.5});

    world.AddObjects(
        {std::make_shared<geometry::Sphere>(s1), std::make_shared<geometry::Sphere>(s2)});

    return world;
}

void scene::World::AddObject(sphereptr object_ptr) {
    objects_.insert(objects_.end(), std::move(object_ptr));
}

void scene::World::AddObjects(std::initializer_list<sphereptr> object_ptrs) {
    for (const auto& object_ptr : object_ptrs) {
        objects_.emplace_back(object_ptr);
    }
}

void scene::World::AddObjects(std::vector<sphereptr>&& sphere_vec) {
    objects_ = sphere_vec;
}

void scene::World::SetLight(std::shared_ptr<lighting::PointLight> light) {
    light_ = std::move(light);
}

bool scene::World::WorldContains(const sphereptr& object) const {
    return std::any_of(begin(objects_), end(objects_), [object](auto o) { return *o == *object; });
}

std::vector<geometry::Intersection> scene::World::Intersect(const commontypes::Ray& ray) const {
    std::vector<geometry::Intersection> intersections{};

    for (const auto& object : objects_) {
        const auto xs = object->Intersect(ray);
        intersections.insert(intersections.end(), xs.begin(), xs.end());
    }

    // return flattened intersections of all objects in ascending order (see rationale on page 93)
    std::sort(intersections.begin(), intersections.end(),
              [](const auto& lhs, const auto& rhs) { return lhs.t_ < rhs.t_; });

    return intersections;
}

commontypes::Color scene::World::ShadeHit(geometry::Computations& comps) const {
    return lighting::Lighting(comps.object_->material(), light_, comps.point_, comps.eye_vector_,
                              comps.normal_vector_);
}

commontypes::Color scene::World::ColorAt(commontypes::Ray& r) const {
    auto intersections = scene::World::Intersect(r);
    auto maybe_hit = geometry::Intersection::Hit(intersections);
    if (maybe_hit.has_value()) {
        auto comps = maybe_hit->PrepareComputations(r);
        return ShadeHit(comps);
    }
    // default color is black when no intersections are present
    return commontypes::Color::Black();
}

bool scene::World::IsShadowed(const commontypes::Point& point) const {
    const commontypes::Vector v = this->light_->position() - point;
    const double distance = v.Magnitude();
    const commontypes::Vector direction = commontypes::Vector{v.Normalize()};
    const commontypes::Ray r{point, direction};

    const auto intersections = this->Intersect(r);
    const auto maybe_hit = geometry::Intersection::Hit(intersections);
    if (maybe_hit.has_value() && maybe_hit.value().t_ < distance) {
        return true;
    }
    return false;
}
