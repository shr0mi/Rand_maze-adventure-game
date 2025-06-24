#include <SFML/Graphics.hpp>
#include "audio.hpp"
#include "pause.hpp"

// Dummy game logic to illustrate (replace with real game logic)
class GameLogic
{
public:
    void update(sf::Time delta) {
        // Game logic updates (e.g., move player, handle physics)
    }

    void draw(sf::RenderWindow& window) {
        // Draw game elements
        sf::CircleShape demo(50);
        demo.setFillColor(sf::Color::Green);
        demo.setPosition({200, 200});
        window.draw(demo);
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 800}), "Game with Pause Menu");
    window.setFramerateLimit(60);

    AudioManager audioManager;
    PauseMenu pauseMenu;
    GameLogic game;

    bool isPaused = false;
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Time delta = clock.restart();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            // Toggle pause on Escape key
            if (event->is<sf::Event::KeyPressed>() &&
                event->get<sf::Event::KeyPressed>().code == sf::Keyboard::Key::Escape)
            {
                isPaused = !isPaused;
            }

            if (isPaused)
            {
                pauseMenu.handleEvent(*event, window, audioManager);

                if (event->is<sf::Event::MouseButtonPressed>())
                {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                    if (pauseMenu.isMenuClicked(mousePos))
                    {
                        if (pauseMenu.isMusicToggled())
                            audioManager.toggleMusic();
                        window.close();
                        //std::system("menu.exe");
                    }

                    if (pauseMenu.isResumeClicked(mousePos))
                    {
                        if (pauseMenu.isMusicToggled())
                            audioManager.toggleMusic();
                        isPaused = false;
                    }
                }
            }
            else
            {
                // Process game events here if needed
            }
        }

        if (!isPaused)
        {
            game.update(delta); // Only update game when not paused
        }

        window.clear();

        if (isPaused)
        {
            pauseMenu.draw(window);
        }
        else
        {
            game.draw(window); // Draw the game normally
        }

        window.display();
    }

    return 0;
}
