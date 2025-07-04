#ifndef VIEW_MANAGER_HPP
#define VIEW_MANAGER_HPP

#include <SFML/Graphics.hpp>

enum class Scene
{
    Menu,
    Options,
    Credits
};

class ViewManager
{
public:
    ViewManager();
    void setView(Scene scene);
    sf::View &getView();

private:
    sf::View view;
};

#endif
