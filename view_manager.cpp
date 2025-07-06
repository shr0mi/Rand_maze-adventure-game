#include "view_manager.hpp"

ViewManager::ViewManager() : view(sf::FloatRect({0, 0}, {960, 540}))
{
    setView(Scene::Menu);
}

void ViewManager::setView(Scene scene)
{
    switch (scene)
    {
    case Scene::Menu:
        view.setCenter({480.f, 270.f}); // Center for the menu screen
        break;
    case Scene::Options:
        view.setCenter({1440.f, 270.f}); // Center for the options screen
        break;
    case Scene::Credits:
        view.setCenter({2400.f, 270.f}); // Center for the credits screen
        break;
    case Scene::Leaderboard:
        view.setCenter({3360.f, 270.f}); // Center for leaderboard screen
        break;
    case Scene::Controls:
        view.setCenter({4320.f, 270.f}); // Center for controls screen
        break;
    }
}

sf::View &ViewManager::getView()
{
    return view;
}
