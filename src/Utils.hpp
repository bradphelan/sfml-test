#pragma once
#include <SFML/Graphics.hpp>
#include <clipper2/clipper.h>
#include <ranges>

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
inline 
polyline make_star(sf::Vector2f const center, double const radius, double const outer, double const inner, int const m)
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
inline
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

inline
/**
 * Draws a polyline on the specified SFML window.
 *
 * @param window The SFML window to draw on.
 * @param polyline The vector of 2D points representing the polyline.
 */
void draw_polyline(sf::RenderWindow& window, const std::vector<sf::Vector2f>& polyline, sf::Color color = sf::Color::White)
{
  auto polyline_shape = sf::VertexArray(sf::LineStrip, polyline.size());

  for (int i = 0; i < polyline.size(); i++)
  {
    polyline_shape[i] = sf::Vertex(polyline[i], color);
  }
  window.draw(polyline_shape);
}

