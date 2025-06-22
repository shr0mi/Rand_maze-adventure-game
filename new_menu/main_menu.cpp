#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "menu.hpp"
#include "options.hpp"
#include "credits.hpp"
#include "view_manager.hpp"
#include "audio.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode({960, 540}), "Main Menu", sf::Style::None);
    window.setFramerateLimit(60);

    ViewManager viewManager;
    MenuScreen menu;
    OptionsScreen options;
    CreditsScreen credits;
    AudioManager audioManager;

    Scene scene = Scene::Menu;

    while (window.isOpen())
    {
        // Handle events
        while (std::optional<sf::Event> event = window.pollEvent())
        {
            // When close button is clicked
            if (event->is<sf::Event::Closed>())
            {
                // Close window
                window.close();
            }
            if (scene == Scene::Options)
            {
                options.handleEvent(*event, window, audioManager);
            }
            if (event->is<sf::Event::MouseButtonPressed>())
            {
                sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (scene == Scene::Menu)
                {
                    if (menu.playClick(pos))
                    {
                        window.close(); // Exit from main window
                        std::system("main.exe");
                    }
                    else if (menu.optionClick(pos))
                    {
                        scene = Scene::Options;
                        viewManager.setView(Scene::Options);
                    }
                    else if (menu.exitClick(pos))
                    {
                        window.close(); // Exit directly
                    }
                }
                else if (scene == Scene::Options)
                {
                    if (options.backClicked(pos))
                    {
                        scene = Scene::Menu;
                        viewManager.setView(Scene::Menu);
                    }
                    else if (options.creditsClicked(pos))
                    {
                        scene = Scene::Credits;
                        viewManager.setView(Scene::Credits);
                    }
                }
                else if (scene == Scene::Credits && credits.backClicked(pos))
                {
                    scene = Scene::Options;
                    viewManager.setView(Scene::Options);
                }
            }
        }

        window.setView(viewManager.getView());
        window.clear();

        if (scene == Scene::Menu)
        {
            menu.draw(window);
        }
        else if (scene == Scene::Options)
        {
            options.draw(window);
        }
        else if (scene == Scene::Credits)
        {
            credits.draw(window);
        }

        window.display();
    }

    return 0;
}
