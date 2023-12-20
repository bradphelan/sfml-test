
#pragma once

#include "DraggableVertex.hpp"
#include "Utils.hpp"
#include <optional>

namespace xtargets {


// A polyline made up of draggable_vertexes
struct draggable_polyline : sf::Drawable
{
    private:
    
    /// The vertices that make up the polyline
    std::vector<draggable_vertex> vertices;
    
    /// The line that is currently being hovered over
    std::optional<int> hoverLine;

    public:
    draggable_polyline(polyline const& points)
    {
        for (auto const& point : points)
        {
            vertices.emplace_back(10, point);
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        for(auto const& vertex : vertices)
        {
            target.draw(vertex, states);
        }   
        auto strip = to_closed_line_strip(to_polyline());
        target.draw(strip, states);

        if(hoverLine)
        {
            int i = *hoverLine;
            int j = (i+1) % vertices.size();
            // Draw a highlight over the line
            auto const& a = vertices[i].getPosition();
            auto const& b = vertices[j].getPosition();

            draw_line(target, a, b, sf::Color::Green);
        }
    }

    /// Split the line that is currently being hovered over at the closest mouse position
    void split_if_hovering(sf::RenderWindow& window)
    {
        if(hoverLine)
        {
            int i = *hoverLine;
            int j = (i+1) % vertices.size();
            auto const& a = vertices[i].getPosition();
            auto const& b = vertices[j].getPosition();
            auto const& mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            auto const& newVertexPosition = closest_point_to_line(a, b, mousePosition);
            vertices.insert(vertices.begin() + j, draggable_vertex(10, newVertexPosition));
        }
    }


    void move(sf::RenderWindow& window, bool & isDragging)
    {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        static int constexpr threshold = 4;

        // Check to see if any of the lines are hovered over
        hoverLine = std::nullopt;
        if(!isDragging)
            for (int i = 0; i < vertices.size(); i++)
            {
                int j = (i + 1) % vertices.size();
                auto dist2 = distance2_mouse_to_line_in_pixels(
                    vertices[i].getPosition(),
                    vertices[j].getPosition(),
                    window);
                if (dist2 < threshold*threshold)
                {
                    hoverLine = i;
                }
            }

        for (auto &vertex : vertices)
            vertex.move(window, isDragging );
    }

    /// return the polyline as a vector of sf::Vector2f
    polyline to_polyline() const
    {
        auto result = polyline{};
        result.reserve(vertices.size());
        for(auto const& vertex : vertices)
        {
            result.push_back(vertex.getPosition());
        }
        return result;
    }


};

}