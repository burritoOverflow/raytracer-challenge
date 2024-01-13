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
    // default property evaluation
    std::shared_ptr<geometry::Shape> s{new geometry::TestShape};
    ASSERT_EQ(s->HasParent(), false);
    ASSERT_EQ(s->GetParent(), nullptr);

    // verify both parts of the parent-child relationship
    geometry::Group g{};
    ASSERT_TRUE(g.GetChildren().empty());
    g.AddChildToGroup(s);

    ASSERT_EQ(s->HasParent(), true);
    ASSERT_EQ(s->GetParent()->id(), g.id());

    const auto children = g.GetChildren();
    ASSERT_FALSE(children.empty());
    ASSERT_EQ(children.size(), 1);
    ASSERT_TRUE(children.at(0) == s);
}

TEST(GroupTest, TestAddingChildToAGroup) {
    geometry::Group g{};
    std::shared_ptr<geometry::Shape> shape_ptr = std::make_shared<geometry::TestShape>();
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

    std::shared_ptr<geometry::Shape> s1_ptr = std::make_shared<geometry::Sphere>();

    std::shared_ptr<geometry::Shape> s2_ptr = std::make_shared<geometry::Sphere>();
    s2_ptr->SetTransform(commontypes::TranslationMatrix{0, 0, -3});

    std::shared_ptr<geometry::Shape> s3_ptr = std::make_shared<geometry::Sphere>();
    s3_ptr->SetTransform(commontypes::TranslationMatrix{5, 0, 0});

    std::initializer_list<std::shared_ptr<geometry::Shape>> children_ptrs{s1_ptr, s2_ptr, s3_ptr};
    g.AddChildrenToGroup(children_ptrs);

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

    std::shared_ptr<geometry::Shape> shape_ptr = std::make_shared<geometry::Sphere>();
    shape_ptr->SetTransform(commontypes::TranslationMatrix{5, 0, 0});
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