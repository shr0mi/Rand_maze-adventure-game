#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
class AudioManager
{
public:
    AudioManager();
    void toggleMusic(); // Toggle music on/off
    void draw(sf::RenderWindow &window) const;
    void setVolume(float volume); // Set the volume of the music
    bool isMusicOn() const; // Check if music is on
    float getVolume() const; // Get the current volume of the music

private:
    sf::Music backgroundMusic;
    bool musicOn = true; // Track music state
};

class SFXManager {
public:
    SFXManager();                 
    void playSound(const std::string& name);  
    void setVolume(float volume);
    float getVolume() const;
    void playSoundWithVolume(const std::string& name, float volume);

private:
    std::unordered_map<std::string, sf::SoundBuffer> buffers;
    std::unordered_map<std::string, sf::Sound> sounds;
    float volume = 50.f;
    
    void loadAllSounds(); 
};

extern SFXManager sfx;