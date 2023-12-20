#pragma once
#include <SFML/Graphics.hpp>

/**
 * Zooms the view at the specified pixel in the given window.
 *
 * @param pixel   The pixel coordinates to zoom at.
 * @param window  The SFML RenderWindow object.
 * @param zoom    The zoom factor.
 */
inline
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
inline
void zoomViewOnScroll(sf::Event event, sf::RenderWindow& window, float zoom)
{
    if (event.type != sf::Event::MouseWheelScrolled)
        return;
    if (event.mouseWheelScroll.delta > 0)
        zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, (1.f / zoom));
    else if (event.mouseWheelScroll.delta < 0)
        zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, zoom);
}