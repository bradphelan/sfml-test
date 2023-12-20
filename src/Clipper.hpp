#pragma once
#include <SFML/Graphics.hpp>
#include <clipper2/clipper.h>
#include <ranges>

namespace xtargets
{


    // A to_vector combinator for std::ranges::views::transform so I can
    // use at the end of a range pipeline to convert the range or view of T to a vector of T.

    inline std::vector<sf::Vector2f> path_to_vector_array(const Clipper2Lib::PathD &path)
    {
        auto result = std::vector<sf::Vector2f>{};
        result.reserve(path.size());
        for (const auto &point : path)
            result.push_back(sf::Vector2f(point.x, point.y));
        return result;
    }

    // implements paths_to_vector_array
    inline std::vector<std::vector<sf::Vector2f>> paths_to_vecvec(const Clipper2Lib::PathsD &paths)
    {
        auto result = std::vector<std::vector<sf::Vector2f>>{};
        result.reserve(paths.size());
        for (const auto &path : paths)
            result.push_back(path_to_vector_array(path));
        return result;
    }

    inline Clipper2Lib::PathD vector_array_to_path(std::vector<sf::Vector2f> const &polyline)
    {
        Clipper2Lib::PathsD result;
        auto path = Clipper2Lib::PathD{};
        path.reserve(polyline.size());
        for (auto const &point : polyline)
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

}