
#pragma once

#include "DraggableVertex.hpp"
#include "Utils.hpp"

// A polyline made up of draggable_vertexes
struct draggable_polyline : sf::Drawable
{
    private:
    std::vector<draggable_vertex> vertices;
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
    }

    void move(sf::RenderWindow& window, bool & isDragging)
    {
        for(auto& vertex : vertices)
        {
            vertex.move(window, isDragging );
        }
    }

    // return the polyline as a vector of sf::Vector2f
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