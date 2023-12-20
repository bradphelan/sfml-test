
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

    draggable_vertex(float radius, sf::Vector2f const& position)
        : sf::CircleShape(radius)
    {
        setPosition(position);
        setOrigin(radius, radius);
        setFillColor(sf::Color::White);
        setOutlineColor(sf::Color::Black);
        setOutlineThickness(1);
    }


    void move(sf::RenderWindow& window)
    {
        sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        bool mouseOver = this->getGlobalBounds().contains(mousePosition);
        // bool mouseOver = mousePosition.x > this->getPosition().x &&
        //        mousePosition.y > this->getPosition().y //
        //        && mousePosition.y < this->getPosition().y + this->getGlobalBounds().height 
        //        && mousePosition.x < this->getPosition().x + this->getGlobalBounds().width 

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
            this->setPosition(mousePosition.x - this->getGlobalBounds().width/2,mousePosition.y - this->getGlobalBounds().height/2);
            if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                this->moving = false;
            }
        }
        else
        {
            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && mouseOver )
            {
                this->moving = true;
            }
        }
    }

};