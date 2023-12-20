
#pragma once

#include "DraggableVertex.hpp"

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

        auto polyline_shape = sf::VertexArray(sf::LineStrip, vertices.size());
        for (int i = 0; i < vertices.size(); i++)
        {
            polyline_shape[i] = sf::Vertex(vertices[i].getPosition());
        }
        target.draw(polyline_shape, states);
    }

    void move(sf::RenderWindow& window)
    {
        for(auto& vertex : vertices)
        {
            vertex.move(window);
        }
    }


};