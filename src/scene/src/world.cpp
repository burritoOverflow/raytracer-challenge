#include "world.h"
#include <algorithm>
#include <utility>
#include "identitymatrix.h"
#include "lighting.h"
#include "scalingmatrix.h"

using shapeptr = std::shared_ptr<geometry::Shape>;

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

void scene::World::AddObject(shapeptr object_ptr) {
    objects_.insert(objects_.end(), std::move(object_ptr));
}

void scene::World::AddObjects(std::initializer_list<shapeptr> object_ptrs) {
    for (const auto& object_ptr : object_ptrs) {
        objects_.emplace_back(object_ptr);
    }
}

void scene::World::AddObjects(std::vector<shapeptr>&& sphere_vec) {
    objects_.insert(objects_.end(), sphere_vec.begin(), sphere_vec.end());
}

void scene::World::SetLight(std::shared_ptr<lighting::PointLight> light) {
    light_ = std::move(light);
}

bool scene::World::WorldContains(const shapeptr& object) const {
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
    const bool shadowed = this->IsShadowed(comps.over_point_);

    const commontypes::Color surface =
        lighting::Lighting(comps.object_->material(), commontypes::IdentityMatrix{}, light_,
                           comps.over_point_, comps.eye_vector_, comps.normal_vector_, shadowed);

    const commontypes::Color reflected_color = ReflectedColor(comps);

    return commontypes::Color{surface + reflected_color};
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

commontypes::Color scene::World::ReflectedColor(geometry::Computations& comps) const {
    const auto material_reflective = comps.object_->material()->Reflective();
    if (material_reflective == 0) {
        return commontypes::Color{0, 0, 0};
    }

    commontypes::Ray reflect_ray{comps.over_point_, comps.reflect_vector_};
    const auto color = ColorAt(reflect_ray);

    return commontypes::Color{color * material_reflective};
}
