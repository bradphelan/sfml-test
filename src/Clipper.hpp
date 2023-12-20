#pragma once
#include <SFML/Graphics.hpp>
#include <clipper2/clipper.h>
#include <ranges>

const double M_PI = 3.14159265358979323846;  /* pi */
using polyline = std::vector<sf::Vector2f>;

/**
 * @brief Creates a star-shaped polyline.
 *
 * This function creates a star-shaped polyline with the specified parameters.
 *
 * @param center The center position of the star.
 * @param radius The radius of the star.
 * @param outer The length of the outer points of the star.
 * @param inner The length of the inner points of the star.
 * @param m The number of points in the star.
 *
 * @return The star-shaped polyline.
 */
inline polyline make_star(sf::Vector2f const center, double const radius, double const outer, double const inner, int const m)
{
    const int n = 2 * m;
    polyline star;
    star.reserve(n + 1);
    for (int i = 0; i != n+1; i++)
    {
        double angle = i * 2 * M_PI / n;
        if (i % 2 == 0)
        {
            star.emplace_back(center.x + radius * cos(angle), center.y + radius * sin(angle));
        }
        else
        {
            star.emplace_back(center.x + outer * cos(angle), center.y + outer * sin(angle));
        }
    }
    return star;
}

/**
 * @brief Creates a polyline representing a box with the specified width and center position.
 * 
 * @param boxWidth The width of the box.
 * @param center The center position of the box.
 * @return The polyline representing the box.
 */
polyline make_box(double boxWidth, sf::Vector2f center)
{
    auto box = polyline{};
    box.reserve(5);
    box.emplace_back(center.x - boxWidth / 2, center.y - boxWidth / 2);
    box.emplace_back(center.x + boxWidth / 2, center.y - boxWidth / 2);
    box.emplace_back(center.x + boxWidth / 2, center.y + boxWidth / 2);
    box.emplace_back(center.x - boxWidth / 2, center.y + boxWidth / 2);
    box.emplace_back(center.x - boxWidth / 2, center.y - boxWidth / 2);
    return box;
}

// A to_vector combinator for std::ranges::views::transform so I can 
// use at the end of a range pipeline to convert the range or view of T to a vector of T.


inline
std::vector<sf::Vector2f> path_to_vector_array(const Clipper2Lib::PathD& path)
{
    auto result = std::vector<sf::Vector2f>{};
    result.reserve(path.size());
    for (const auto &point : path)
        result.push_back(sf::Vector2f(point.x, point.y));
    return result;
}

// implements paths_to_vector_array
inline  std::vector<std::vector<sf::Vector2f>> paths_to_vecvec(const Clipper2Lib::PathsD& paths)
{
    auto result = std::vector<std::vector<sf::Vector2f>>{};
    result.reserve(paths.size());
    for (const auto &path : paths)
        result.push_back(path_to_vector_array(path));
    return result;
}

inline Clipper2Lib::PathD vector_array_to_path(std::vector<sf::Vector2f> const& polyline)
{
    Clipper2Lib::PathsD result;
    auto path = Clipper2Lib::PathD{};
    path.reserve(polyline.size());
    for (auto const& point : polyline)
    {
        path.push_back(Clipper2Lib::PointD{point.x, point.y});
    }
    return path;
}

// Clip a vector array with a vector array
inline std::vector<std::vector<sf::Vector2f>> clip(std::vector<sf::Vector2f> const &subject, std::vector<sf::Vector2f> const &clip, Clipper2Lib::ClipType clipType)
{
    auto subject_path = vector_array_to_path(subject);
    auto clip_path = vector_array_to_path(clip);
    auto result = Clipper2Lib::BooleanOp(Clipper2Lib::ClipType::Intersection, Clipper2Lib::FillRule::NonZero, {subject_path}, {clip_path});
    return paths_to_vecvec(std::move(result));
}
