#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "View-Based Menu");
    window.setFramerateLimit(60);

    // Create a view representing the camera
    sf::View view(sf::FloatRect({0, 0}, {800, 600})); // Starts at (0,0)

    // Menu Button
    sf::RectangleShape btnOptions(sf::Vector2f({200, 50}));
    btnOptions.setPosition({300, 250});
    btnOptions.setFillColor(sf::Color::Green);

    // Options "Back" Button
    sf::RectangleShape btnBack(sf::Vector2f({200, 50}));
    btnBack.setPosition({1100, 250}); // Located in Options view area
    btnBack.setFillColor(sf::Color::Red);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (event->is<sf::Event::MouseButtonPressed>())
            {
                sf::Vector2f mouseWorldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // In Menu screen
                if (view.getCenter().x == 400 && btnOptions.getGlobalBounds().contains(mouseWorldPos)) {
                    view.setCenter({1200, 300}); // Move view to Options screen (offset by 800)
                }

                // In Options screen
                if (view.getCenter().x == 1200 && btnBack.getGlobalBounds().contains(mouseWorldPos)) {
                    view.setCenter({400, 300}); // Move back to Menu
                }
            }
        }

        window.setView(view);
        window.clear(sf::Color::Black);

        // Draw everything (both views exist in world, but only one is visible)
        window.draw(btnOptions);
        window.draw(btnBack);

        window.display();
    }

    return 0;
}