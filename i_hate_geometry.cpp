#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 900), "ChronoSpacer");
    // Initialise everything below
    sf::CircleShape circle;
    circle.setRadius(100);
    circle.setFillColor(sf::Color::Red);

    // Game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Process any input event here
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();
        // Whatever I want to draw goes here
        window.draw(circle);
        window.display();
    }
}