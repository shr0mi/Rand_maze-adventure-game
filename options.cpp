#include "options.hpp"
#include "audio.hpp"

SliderVolume::SliderVolume(float x, float y, float width)
    : isDragging(false), volumeText(font, "", 18) // Initialize the font and text size
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
        // error
    }

    volumeText.setFillColor(sf::Color::White);

    // Initial knob position to match default volume
    float defaultVolume = 20.f;
    float initialX = minX + (defaultVolume / 100.f) * (maxX - minX);
    knob.setPosition({initialX, y + bar.getSize().y / 2}); // Set initial knob position based on default volume

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

            // Check if the mouse is pressed on the knob
            if (knob.getGlobalBounds().contains(mousePos))
            {
                isDragging = true; 
            }
        }
    }
    else if (const auto *mouseReleased = event.getIf<sf::Event::MouseButtonReleased>())
    {
        // Stop dragging if the left mouse button is released
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
    int volume = static_cast<int>(getVolume()); // Get volume as an integer
    volumeText.setString(std::to_string(volume));

    // Center text above knob
    sf::FloatRect bounds = volumeText.getLocalBounds();
    volumeText.setOrigin({bounds.size.x / 2, bounds.size.y});
    volumeText.setPosition({knob.getPosition().x, knob.getPosition().y - 20});
}

void SliderVolume::updateKnobPosition(float mouseX)
{
    // Clamp the mouse position to the slider's range
    float x = std::clamp(mouseX, minX, maxX);
    knob.setPosition({x, bar.getPosition().y + bar.getSize().y / 2});
    updateText();

    // Update the volume text position
    if (audio)
    {
        audio->setVolume(getVolume());
    }
}

float SliderVolume::getVolume() const
{
    float position = knob.getPosition().x;
    float volume = ((position - minX) / (maxX - minX)) * 100.0f;
    return volume;
}

void SliderVolume::setVolume(float volume)
{
    // Ensure volume is within the range [0, 100]
    float x = minX + (std::clamp(volume, 0.f, 100.f) / 100.f) * (maxX - minX);
    knob.setPosition({x, bar.getPosition().y + bar.getSize().y / 2});
    updateText();
}

void SliderVolume::draw(sf::RenderWindow &window) const
{
    window.draw(bar);
    window.draw(knob);
    window.draw(volumeText);
}

OptionsScreen::OptionsScreen(AudioManager &audioManager) : bgTexture("assets/menubg.png"),
                                                           optionTexture("assets/ui_options_title.png"),
                                                           musicTexture("assets/musicbtn.png"),
                                                           backTexture("assets/backbtn.png"),
                                                           creditsTexture("assets/ui_credits_btn.png"),
                                                           musicOnTexture("assets/ui_music_btn_on.png"),
                                                           musicOffTexture("assets/ui_music_btn_off.png"),

                                                           background(bgTexture),
                                                           optionImage(optionTexture),
                                                           musicImage(musicTexture),
                                                           backButton(backTexture),
                                                           creditsButton(creditsTexture),
                                                           musicOnButton(musicOnTexture),
                                                           musicOffButton(musicOffTexture),

                                                           slider(1250, 150, 400) // Initialize the slider with position (1250, 150) and width 400
{
    background.setPosition({960, 0});

    optionImage.setPosition({1250, 10});
    optionImage.setScale({0.2f, 0.2f});

    musicImage.setPosition({1150, 115});
    musicImage.setScale({0.3f, 0.3f});

    backButton.setPosition({1350, 400});
    backButton.setScale({0.3f, 0.3f});

    creditsButton.setPosition({1350, 300});
    creditsButton.setScale({0.2f, 0.2f});

    musicOnButton.setPosition({1300, 200});
    musicOnButton.setScale({0.2f, 0.2f});

    musicOffButton.setPosition({1300, 200});
    musicOffButton.setScale({0.2f, 0.2f});

    slider.setAudioManager(&audioManager);

    this->audio = &audioManager;
}

void OptionsScreen::syncWithAudio(AudioManager &audioManager)
{
    // Sync the slider with the current audio volume
    slider.setVolume(audioManager.getVolume());
}

void OptionsScreen::handleEvent(const sf::Event &event, const sf::RenderWindow &window, AudioManager &audioManager)
{
    slider.handleEvent(event, window);
    audioManager.setVolume(slider.getVolume());

    if (event.is<sf::Event::MouseButtonPressed>())
    {
        sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (musicOnButton.getGlobalBounds().contains(pos) || musicOffButton.getGlobalBounds().contains(pos))
        {
            audioManager.toggleMusic(); // Toggle music on/off
            syncWithAudio(audioManager); // Update slider and toggle display
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
    window.draw(optionImage);
    window.draw(musicImage);
    window.draw(creditsButton);
    window.draw(backButton);

    // Draw the music button based on the current state
    if (audio && audio->isMusicOn())
    {
        window.draw(musicOnButton);
    }
    else
    {
        window.draw(musicOffButton);
    }

    slider.draw(window);
}
