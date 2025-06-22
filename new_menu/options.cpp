#include "options.hpp"

SliderVolume::SliderVolume(float x, float y, float width)
    : isDragging(false), volumeText(font, "", 18)
{
    bar.setSize(sf::Vector2f(width, 5));
    bar.setFillColor(sf::Color(150, 150, 255));
    bar.setPosition({x, y});

    knob.setRadius(10);
    knob.setFillColor(sf::Color::Red);
    knob.setOrigin({knob.getRadius(), knob.getRadius()});
    knob.setPosition({x + width / 2, y + bar.getSize().y / 2});

    minX = x;
    maxX = x + width;

    if (!font.openFromFile("assets/arial.ttf"))
    {
        // Fallback: just ignore text rendering!
    }

    volumeText.setFillColor(sf::Color::White);
    updateText();
}

void SliderVolume::handleEvent(const sf::Event &event, const sf::RenderWindow &window)
{
    if (const auto *mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePressed->button == sf::Mouse::Button::Left)
        {
            sf::Vector2i pixel(mousePressed->position.x, mousePressed->position.y);
            sf::Vector2f mousePos = window.mapPixelToCoords(pixel);
            if (knob.getGlobalBounds().contains(mousePos))
            {
                isDragging = true;
            }
        }
    }
    else if (const auto *mouseReleased = event.getIf<sf::Event::MouseButtonReleased>())
    {
        if (mouseReleased->button == sf::Mouse::Button::Left)
        {
            isDragging = false;
        }
    }
    else if (const auto *mouseMoved = event.getIf<sf::Event::MouseMoved>())
    {
        if (isDragging)
        {
            sf::Vector2i pixel(mouseMoved->position.x, mouseMoved->position.y);
            sf::Vector2f mousePos = window.mapPixelToCoords(pixel);
            updateKnobPosition(mousePos.x);
        }
    }
}

void SliderVolume::updateText()
{
    int volume = static_cast<int>(getVolume());
    volumeText.setString(std::to_string(volume));

    // Center text above knob
    sf::FloatRect bounds = volumeText.getLocalBounds();
    volumeText.setOrigin({bounds.size.x / 2, bounds.size.y});
    volumeText.setPosition({knob.getPosition().x, knob.getPosition().y - 20});
}

void SliderVolume::updateKnobPosition(float mouseX)
{
    float x = std::clamp(mouseX, minX, maxX);
    knob.setPosition({x, bar.getPosition().y + bar.getSize().y / 2});
    updateText();
}

float SliderVolume::getVolume() const
{
    float position = knob.getPosition().x;
    float volume = ((position - minX) / (maxX - minX)) * 100.0f;
    return volume;
}

void SliderVolume::draw(sf::RenderWindow &window) const
{
    window.draw(bar);
    window.draw(knob);
    window.draw(volumeText);
}

OptionsScreen::OptionsScreen() : bgTexture("assets/menubg.png"),
                                 backTexture("assets/backbtn.png"),
                                 creditsTexture("assets/ui_credits_btn.png"),
                                 musicOnTexture("assets/ui_music_btn_on.png"),
                                 musicOffTexture("assets/ui_music_btn_off.png"),

                                 background(bgTexture),
                                 backButton(backTexture),
                                 creditsButton(creditsTexture),
                                 musicButton(musicOnTexture),

                                 slider(1150, 150, 200)
{
    background.setPosition({960, 0});

    backButton.setPosition({1300, 400});
    backButton.setScale({0.3f, 0.3f});

    creditsButton.setPosition({1300, 250});
    creditsButton.setScale({0.2f, 0.2f});

    musicButton.setPosition({250, 300});
    musicButton.setScale({0.2f, 0.2f});
}

void OptionsScreen::handleEvent(const sf::Event &event, const sf::RenderWindow &window, AudioManager &audioManager)
{
    slider.handleEvent(event, window);
    audioManager.setVolume(slider.getVolume());

    if (event.is<sf::Event::MouseButtonPressed>())
    {
        sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (musicButton.getGlobalBounds().contains(pos))
        {
            audioManager.toggleMusic();
            musicOn = !musicOn;
            musicButton.setTexture(musicOn ? musicOnTexture : musicOffTexture);
        }
    }
}

bool OptionsScreen::backClicked(sf::Vector2f pos)
{
    return backButton.getGlobalBounds().contains(pos);
}

bool OptionsScreen::creditsClicked(sf::Vector2f pos)
{
    return creditsButton.getGlobalBounds().contains(pos);
}

void OptionsScreen::draw(sf::RenderWindow &window)
{
    window.draw(background);
    window.draw(creditsButton);
    window.draw(backButton);
    window.draw(musicButton);
    slider.draw(window);
}
