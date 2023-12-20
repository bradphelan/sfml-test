#include <SFML/Graphics.hpp>
#include <clipper2/clipper.h>
#include "Clipper.hpp"
#include <mutex>

int main()
{
    auto window = sf::RenderWindow{ { 1920u, 1080u }, "CMake SFML Project" };
    window.setFramerateLimit(144);

    // Zoom the view by this amount when scroll wheeling
    double zoomAmount=1.1;

    // initial number of star points
    int star_points = 6;

    std::once_flag on_startup;

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
        // Draw a circle with radius 1 at (0,0)
        auto circle = sf::CircleShape(100);
        circle.setPosition(0, 0);
        circle.setOrigin(100, 100);
        circle.setFillColor(sf::Color::Transparent);    
        circle.setOutlineColor(sf::Color::Red);
        circle.setOutlineThickness(1);
        window.draw(circle);

        auto circleCenter = circle.getPosition();

        // Make a box
        auto box = make_box(200, circleCenter);

        // Make a star
        auto star = make_star(circleCenter, 100, 200, 50, star_points);


        
        draw_xy_axis(window, circleCenter, 200);
        draw_polyline(window, box);
        draw_polyline(window, star);

        window.draw(circle);


        window.display();
    }
}