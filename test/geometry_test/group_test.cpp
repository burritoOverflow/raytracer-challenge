#include "group.h"
#include <gtest/gtest.h>
#include "scalingmatrix.h"
#include "sphere.h"
#include "test_classes.h"
#include "translationmatrix.h"

TEST(GroupTest, TestCreatingNewGroup) {
    const geometry::Group g{};
    ASSERT_TRUE(g.GetTransform() ==
                commontypes::IdentityMatrix());  // set in the Shape's default ctor
}

TEST(GroupTest, TestShapeHasParentAttribute) {
    const geometry::TestShape s{};
    ASSERT_EQ(s.HasParent(), false);
    ASSERT_EQ(s.GetParent(), nullptr);
}

TEST(GroupTest, TestAddingChildToAGroup) {
    geometry::Group g{};
    geometry::TestShape s{};
    std::shared_ptr<geometry::Shape> shape_ptr =
        std::make_shared<geometry::TestShape>(std::move(s));
    g.AddChildToGroup(shape_ptr);

    ASSERT_NE(g.GetChildren().size(), 0);
    ASSERT_EQ(g.GetChildren().size(), 1);

    const std::vector<std::shared_ptr<geometry::Shape>> group_children = g.GetChildren();

    // ensure the shape above is present
    const auto result =
        std::find_if(group_children.begin(), group_children.end(),
                     [&shape_ptr](const std::shared_ptr<geometry::Shape>& shape_ptr2) {
                         return shape_ptr->id() == shape_ptr2->id();
                     });

    ASSERT_NE(result, group_children.end());
    ASSERT_EQ(shape_ptr->GetParent()->id(), g.id());
}

TEST(GroupTest, TestIntersectingRayWithEmptyGroup) {
    const geometry::Group g{};
    const commontypes::Ray r =
        commontypes::Ray{commontypes::Point{0, 0, 0}, commontypes::Vector{0, 0, 1}};
    auto xs = g.LocalIntersect(r);
    ASSERT_EQ(xs.size(), 0);
}

TEST(GroupTest, TestIntersectingRayWithNonEmptyGroup) {
    geometry::Group g{};

    geometry::Sphere s1{};
    std::shared_ptr<geometry::Shape> s1_ptr = std::make_shared<geometry::Sphere>(std::move(s1));

    geometry::Sphere s2{};
    s2.SetTransform(commontypes::TranslationMatrix{0, 0, -3});
    std::shared_ptr<geometry::Shape> s2_ptr = std::make_shared<geometry::Sphere>(std::move(s2));

    geometry::Sphere s3{};
    s3.SetTransform(commontypes::TranslationMatrix{5, 0, 0});
    std::shared_ptr<geometry::Shape> s3_ptr = std::make_shared<geometry::Sphere>(std::move(s3));

    g.AddChildToGroup(s1_ptr);
    g.AddChildToGroup(s2_ptr);
    g.AddChildToGroup(s3_ptr);

    const commontypes::Ray r =
        commontypes::Ray{commontypes::Point{0, 0, -5}, commontypes::Vector{0, 0, 1}};

    const auto xs = g.LocalIntersect(r);

    // pointer equality won't work as the `LocalIntersect` methods create shared pointers and add
    // to the local Intersection
    ASSERT_EQ(xs.at(0).object_->id(), s2_ptr->id());
    ASSERT_EQ(xs.at(1).object_->id(), s2_ptr->id());
    ASSERT_EQ(xs.at(2).object_->id(), s1_ptr->id());
    ASSERT_EQ(xs.at(3).object_->id(), s1_ptr->id());
}

TEST(GroupTest, TestIntersectingATransformedGroup) {
    geometry::Group g{};
    g.SetTransform(commontypes::ScalingMatrix{2, 2, 2});
    geometry::Sphere s{};
    s.SetTransform(commontypes::TranslationMatrix{5, 0, 0});
    std::shared_ptr<geometry::Shape> shape_ptr = std::make_shared<geometry::Sphere>(std::move(s));

    g.AddChildToGroup(shape_ptr);
    const commontypes::Ray r =
        commontypes::Ray{commontypes::Point{10, 0, -10}, commontypes::Vector{0, 0, 1}};
    const auto xs = g.Intersect(r);

    ASSERT_EQ(xs.size(), 2);
}

TEST(GroupTest, TestLocalNormalAtThrowsException) {
    geometry::Group g{};
    // method should not be invoked directly (see pg. 200)
    EXPECT_THROW(g.LocalNormalAt(commontypes::Point(1, 2, 3)), IncorrectCallException);
}