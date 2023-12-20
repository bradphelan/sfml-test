#include <SFML/Graphics.hpp>
#include <clipper2/clipper.h>
#include "Clipper.hpp"
#include "DraggableVertex.hpp"
#include <mutex>


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

int main()
{
    auto window = sf::RenderWindow{ { 1920u, 1080u }, "CMake SFML Project" };
    window.setFramerateLimit(144);

    // Zoom the view by this amount when scroll wheeling
    double zoomAmount=1.1;

    // initial number of star points
    int star_points = 6;

    std::once_flag on_startup;

    // Create a box draggable_polyline
    auto box = draggable_polyline(make_box(200, sf::Vector2f(0,0)));

    // Create a star draggable_polyline
    auto star = draggable_polyline(make_star(sf::Vector2f(0,0), 100, 200, 50, star_points));

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
                star_points++;

            zoomViewOnScroll(event, window, zoomAmount);
        }

        // Center the screen at (0,0)
        std::call_once(on_startup, [&] {
            auto view = window.getView();
            view.setCenter(0, 0);
            window.setView(view);
        });

        window.clear();

        box.move(window);
        box.draw(window, sf::RenderStates::Default);

        star.move(window);
        star.draw(window, sf::RenderStates::Default);

        window.display();
    }
}