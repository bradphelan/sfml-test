#pragma once
#include <SFML/Graphics.hpp>
#include <clipper2/clipper.h>
#include <ranges>

namespace xtargets {

using polyline = std::vector<sf::Vector2f>;

const double M_PI = 3.14159265358979323846; /* pi */

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
    star.reserve(n );
    for (int i = 0; i != n; i++)
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
    return box;
}


inline
/**
 * Converts a polyline into a closed line strip.
 *
 * This function takes a vector of 2D points representing a polyline and converts it into a closed line strip.
 * A closed line strip is a continuous line that connects the points in the polyline, forming a closed shape.
 *
 * @param polyline The vector of 2D points representing the polyline.
 * @param color The color of the line strip (optional, default is sf::Color::White).
 * @return A closed line strip shape with the specified color.
 */
auto to_closed_line_strip(const std::vector<sf::Vector2f> & polyline, sf::Color color = sf::Color::White)
{
    auto result = sf::VertexArray(sf::LineStrip, polyline.size()+1);
    for (int i = 0; i < polyline.size(); i++)
        result[i] = sf::Vertex(polyline[i], color);
    result[polyline.size()]=sf::Vertex(polyline[0], color);
    return result;
}


/**
 * Draws a polyline on the specified SFML window.
 *
 * @param window The SFML window to draw on.
 * @param polyline The vector of 2D points representing the polyline.
 */
inline
void draw_polyline(sf::RenderWindow& window, const std::vector<sf::Vector2f>& polyline, sf::Color color = sf::Color::White)
{

  window.draw(to_closed_line_strip(polyline, color));
}

/**
 * @brief Draws a line on the specified SFML window.
 *
 * @param window The SFML window to draw on.
 * @param a The starting point of the line.
 * @param b The ending point of the line.
 * @param color The color of the line (default: sf::Color::White).
 */
inline void draw_line(sf::RenderTarget& target, sf::Vector2f const& a, sf::Vector2f const& b, sf::Color color = sf::Color::White)
{
    target.draw(to_closed_line_strip({a,b}, color));
}

/// Vector dot product
template <typename T>
inline
T dot(sf::Vector2<T> const & a, sf::Vector2<T> const & b)
{
    return a.x * b.x + a.y * b.y;
}

/// Distance of point p to line segment (a,b)
template<typename T>
sf::Vector2<T> closest_point_to_line(sf::Vector2<T> const & a, sf::Vector2<T> const & b, sf::Vector2<T> const & p)
{
    sf::Vector2<T> const  ab = b - a;
    sf::Vector2<T> const ap = p - a;
    double const abap = dot(ab, ap);
    double const  abab = dot(ab, ab);
    double t = abap / abab;
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    if constexpr (std::is_integral_v<T>)
        return a + sf::Vector2<T>((T)std::round(t * ab.x), (T)std::round(t * ab.y));
    else
        return a + sf::Vector2<T>(t * ab.x, t * ab.y);
}

/// Distance of point p to line segment (a,b)
/// Returns integer value if the template parameter is an integer type
template<typename T>
T distance2_to_line(sf::Vector2<T> const & a, sf::Vector2<T> const & b, sf::Vector2<T> const & p)
{
    sf::Vector2<T> const closest = closest_point_to_line(a, b, p);
    sf::Vector2<T> const delta = p - closest;
    return dot(delta, delta);
}

/**
 * Calculates the squared distance between the mouse position and a line segment defined by two points in pixels. The
 * input points are in world coordinates, but the mouse position is in pixel coordinates. This function converts the
 * world coordinates to pixel coordinates before calculating the distance.
 *
 * @param aWorld The first point of the line segment in world coordinates.
 * @param bWorld The second point of the line segment in world coordinates.
 * @param window The SFML RenderWindow object representing the window.
 * @return The squared distance between the mouse position and the line segment.
 */
int distance2_mouse_to_line_in_pixels(sf::Vector2f const &aWorld, sf::Vector2f const &bWorld, sf::RenderWindow const &window)
{

    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    auto const &a = window.mapCoordsToPixel(aWorld);
    auto const &b = window.mapCoordsToPixel(bWorld);
    auto cptl = closest_point_to_line(a, b, mousePosition);
    auto delta = mousePosition - cptl;
    return dot(delta,delta);
}

}