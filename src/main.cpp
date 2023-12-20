#include <SFML/Graphics.hpp>
#include <clipper2/clipper.h>
#include "Clipper.hpp"
#include "DraggablePolyline.hpp"
#include "Utils.hpp"
#include "Zoom.hpp"
#include <mutex>
#include <SelbaWard.hpp>

// Triangulate the polyline using ear clipping


int main()
{
    auto window = sf::RenderWindow{ { 1920u, 1080u }, "CMake SFML Project" };
    window.setFramerateLimit(144);

    // Zoom the view by this amount when scroll wheeling
    double zoomAmount=1.1;

    // Flag to run things on startup. Could use a boolean
    // but this is more fun.
    std::once_flag on_startup;

    // Create a box draggable_polyline
    auto box = draggable_polyline(make_box(200, sf::Vector2f(0,0)));

    // Create a star draggable_polyline
    auto star = draggable_polyline(make_star(sf::Vector2f(300,0), 100, 200, 50, 6));

    // Flag to indicate if we are dragging a vertex. This is required
    // because we need to know if we are dragging a vertex or not otherwise
    // we will pick up other vertexes.
    bool isDragging = false;

    while (window.isOpen())
    {
        bool mouseClickRight = false;
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                // check if right mouse button is pressed
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    mouseClickRight = true;
                }
            }

            zoomViewOnScroll(event, window, zoomAmount);
        }

        // Center the screen at (0,0)
        std::call_once(on_startup, [&] {
            auto view = window.getView();
            view.setCenter(0, 0);
            window.setView(view);
        });

        window.clear();

        box.move(window, isDragging);
        window.draw(box);
        if(mouseClickRight)
            box.split(window);

        star.move(window, isDragging);
        window.draw(star);
        if(mouseClickRight)
            star.split(window);

        auto clipped = clip(box.to_polyline(), star.to_polyline(), Clipper2Lib::ClipType::Intersection);
        for (auto const &path : clipped)
            draw_polyline(window, path, sf::Color::Red);

        window.display();
    }
}