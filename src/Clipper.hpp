#pragma once
#include <SFML/Graphics.hpp>
#include <clipper2/clipper.h>

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

inline
auto paths_to_vertex_array(const Clipper2Lib::PathsD& paths)
{
  auto result = std::vector<sf::Vertex>{};
  for (const auto& path : paths)
  {
    for (const auto& point : path)
    {
      result.push_back(sf::Vertex(sf::Vector2f(point.x, point.y)));
    }
  }
  // Close the path
    result.push_back(sf::Vertex(sf::Vector2f(paths[0][0].x, paths[0][0].y)));
  return result;
}

inline
/**
 * Draws a polyline on the specified SFML window.
 *
 * @param window The SFML window to draw on.
 * @param polyline The vector of 2D points representing the polyline.
 */
void draw_polyline(sf::RenderWindow& window, const std::vector<sf::Vector2f>& polyline)
{
  auto polyline_shape = sf::VertexArray(sf::LineStrip, polyline.size());
  for (int i = 0; i < polyline.size(); i++)
  {
    polyline_shape[i] = sf::Vertex(polyline[i]);
  }
  window.draw(polyline_shape);
}

inline
/**
 * Draws the X and Y axes on the given SFML window.
 *
 * @param window The SFML window to draw on.
 * @param center The center point of the axes.
 * @param length The length of the axes.
 */
void draw_xy_axis(sf::RenderWindow& window, sf::Vector2f center, double length)
{
  auto x_axis = std::vector<sf::Vector2f>{};
  x_axis.push_back(sf::Vector2f(center.x - length / 2, center.y));
  x_axis.push_back(sf::Vector2f(center.x + length / 2, center.y));
  draw_polyline(window, x_axis);

  auto y_axis = std::vector<sf::Vector2f>{};
  y_axis.push_back(sf::Vector2f(center.x, center.y - length / 2));
  y_axis.push_back(sf::Vector2f(center.x, center.y + length / 2));
  draw_polyline(window, y_axis);
}


/**
 * Zooms the view at the specified pixel in the given window.
 *
 * @param pixel   The pixel coordinates to zoom at.
 * @param window  The SFML RenderWindow object.
 * @param zoom    The zoom factor.
 */
void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom)
{
	const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixel) };
	sf::View view{ window.getView() };
	view.zoom(zoom);
	window.setView(view);
	const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel) };
	const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
	view.move(offsetCoords);
	window.setView(view);
}

/**
 * Zooms the view of the window on scroll event.
 *
 * This function adjusts the view of the specified window based on the scroll event
 * and the specified zoom factor. Just pass in the current window event and 
 * it will be detected if the scroll wheel was used and zoom the view accordingly.
 *
 * @param event   The current window event
 * @param window  The SFML RenderWindow object.
 * @param zoom    The zoom factor to apply to the view.
 */
void zoomViewOnScroll(sf::Event event, sf::RenderWindow& window, float zoom)
{
    if (event.type != sf::Event::MouseWheelScrolled)
        return;
    if (event.mouseWheelScroll.delta > 0)
        zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, (1.f / zoom));
    else if (event.mouseWheelScroll.delta < 0)
        zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, zoom);
}

