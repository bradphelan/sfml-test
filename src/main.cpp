#include <SFML/Graphics.hpp>
#include <clipper2/clipper.h>

auto test_clip()
{
  Clipper2Lib::Paths64 subject, clip, solution;
  subject.push_back(Clipper2Lib::MakePath({100, 50, 10, 79, 65, 2, 65, 98, 10, 21}));
  clip.push_back(Clipper2Lib::MakePath({98, 63, 4, 68, 77, 8, 52, 100, 19, 12}));
  return Clipper2Lib::Intersect(subject, clip, Clipper2Lib::FillRule::NonZero);    
}  

// Convert Clipper2Lib::Paths64 to std::vector<sf::Vertex>
auto paths_to_vertex_array(const Clipper2Lib::Paths64& paths)
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

// Create a box with center at center and width and height equal to boxWidth
std::vector<sf::Vertex> CreateBox(double boxWidth, sf::Vertex center)
{
    auto box = std::vector<sf::Vertex>{};
    box.push_back(sf::Vertex(sf::Vector2f(center.position.x - boxWidth / 2, center.position.y - boxWidth / 2)));
    box.push_back(sf::Vertex(sf::Vector2f(center.position.x + boxWidth / 2, center.position.y - boxWidth / 2)));
    box.push_back(sf::Vertex(sf::Vector2f(center.position.x + boxWidth / 2, center.position.y + boxWidth / 2)));
    box.push_back(sf::Vertex(sf::Vector2f(center.position.x - boxWidth / 2, center.position.y + boxWidth / 2)));
    box.push_back(sf::Vertex(sf::Vector2f(center.position.x - boxWidth / 2, center.position.y - boxWidth / 2)));
    return box;
}


int main()
{
    auto window = sf::RenderWindow{ { 1920u, 1080u }, "CMake SFML Project" };
    window.setFramerateLimit(144);
    double zoomAmount=1.1;
    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (event.mouseWheelScroll.delta > 0)
                    zoomViewAt({event.mouseWheelScroll.x, event.mouseWheelScroll.y}, window, (1.f / zoomAmount));
                else if (event.mouseWheelScroll.delta < 0)
                    zoomViewAt({event.mouseWheelScroll.x, event.mouseWheelScroll.y}, window, zoomAmount);
            }
        }


        // Center the screen at (0,0)
        auto view = window.getView();
        view.setCenter(0, 0);
        window.setView(view);

        window.clear();
        // Draw a circle with radius 1 at (0,0)
        auto circle = sf::CircleShape(100);
        circle.setPosition(0, 0);
        circle.setOrigin(100, 100);
        circle.setFillColor(sf::Color::Transparent);    
        circle.setOutlineColor(sf::Color::Red);
        circle.setOutlineThickness(1);
        window.draw(circle);

        auto circleCenter = sf::Vertex(circle.getPosition());

        // Draw a box around the circle
        auto box = CreateBox(200, circleCenter);
        window.draw(box.data(), box.size(), sf::LineStrip);

        // Draw the x,y axis
        auto xAxis = std::vector<sf::Vertex>{};
        xAxis.push_back(sf::Vertex(sf::Vector2f(0, 0)));
        xAxis.push_back(sf::Vertex(sf::Vector2f(1920, 0)));
        window.draw(xAxis.data(), xAxis.size(), sf::LineStrip);

        auto yAxis = std::vector<sf::Vertex>{};
        yAxis.push_back(sf::Vertex(sf::Vector2f(0, 0)));
        yAxis.push_back(sf::Vertex(sf::Vector2f(0, 1080)));
        window.draw(yAxis.data(), yAxis.size(), sf::LineStrip);

        // Draw the output of test_clip
        auto result = test_clip();
        auto resultVertexArray = paths_to_vertex_array(result);
        window.draw(resultVertexArray.data(), resultVertexArray.size(), sf::LineStrip);

        window.draw(circle);



        window.display();
    }
}