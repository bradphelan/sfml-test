#include <gtest/gtest.h>
#include "../Utils.hpp"
#include "../Clipper.hpp"

using namespace xtargets;

TEST(Booleans, NoIntersection)
{
    auto box0 = make_box(200, sf::Vector2f(0,0));
    auto box1 = make_box(200, sf::Vector2f(201,0));

    auto result = clip(box0, box1, Clipper2Lib::ClipType::Intersection);

    EXPECT_EQ(result.size(), 0);
}

TEST(Booleans, Intersection)
{
    auto box0 = make_box(200, sf::Vector2f(0,0));
    auto box1 = make_box(200, sf::Vector2f(50,50));

    auto result = clip(box0, box1, Clipper2Lib::ClipType::Intersection);

    EXPECT_EQ(result.size(), 1);
}


TEST(Orientation, CCW)
{
    // A counter clockwise seq
    std::vector<sf::Vector2f> points = {
        {0,0},
        {1,0},
        {1,1}
    };

    EXPECT_EQ(orientation(points[0], points[1], points[2]), Orientation::CounterClockwise);
}

TEST(Orientation, CW)
{
    // A clockwise seq
    std::vector<sf::Vector2f> points = {
        {0,0},
        {0,1},
        {1,1}
    };

    EXPECT_EQ(orientation(points[0], points[1], points[2]), Orientation::Clockwise);
}