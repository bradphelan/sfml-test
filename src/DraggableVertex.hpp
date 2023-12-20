
#include <SFML/Graphics.hpp>
#include <clipper2/clipper.h>
#include "Clipper.hpp"
#include <mutex>

// A draggable vertex object that listens
// for mouse press events and sets a dragging
// flag. When the flag is dragging is true, the
// vertex will follow the mouse position until
// the mouse is released.
struct draggable_vertex : sf::CircleShape
{
    bool moving = false;
    sf::Vector2f offset;

    inline
    draggable_vertex(float radius, sf::Vector2f const& position)
        : sf::CircleShape(radius)
    {
        setPosition(position);
        setOrigin(radius, radius);
        setFillColor(sf::Color::White);
        setOutlineColor(sf::Color::Black);
        setOutlineThickness(1);
    }


    inline
    void move(sf::RenderWindow& window, bool & isDragging)
    {
        sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        bool mouseOver = this->getGlobalBounds().contains(mousePosition);

        if(mouseOver)
        {
            this->setOutlineColor(sf::Color::Red);
            this->setFillColor(sf::Color::Green);
            setOutlineThickness(3);
        }
        else
        {
            this->setOutlineColor(sf::Color::Blue);
            this->setFillColor(sf::Color::White);
            setOutlineThickness(1);
        }

        if(this->moving)
        {
            this->setPosition(mousePosition.x , mousePosition.y );
            if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                this->moving = false;
                isDragging = false;
            }
        }
        else
        {
            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && mouseOver && !isDragging )
            {
                this->moving = true;
                isDragging = true;
            }
        }
    }

};