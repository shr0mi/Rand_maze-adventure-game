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
        view.setCenter({480.f, 270.f});
        break;
    case Scene::Options:
        view.setCenter({1440.f, 270.f});
        break;
    case Scene::Credits:
        view.setCenter({2400.f, 270.f});
        break;
    }
}

sf::View &ViewManager::getView()
{
    return view;
}
