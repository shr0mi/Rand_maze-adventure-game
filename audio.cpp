#include "options.hpp"
#include "audio.hpp"
#include <iostream>

AudioManager::AudioManager()
{
    if (!backgroundMusic.openFromFile("assets/One_Man_Symphony_In_The_Blink_Of_An_Eye_(Free)_02_An_Instant_Burst_(Action_02).mp3"))
    {
        std::cerr << "Failed to load music.\n";
    }
    else
    {
        backgroundMusic.setLooping(true);
        backgroundMusic.setVolume(15.f); // Set default volume
        backgroundMusic.play();
    }

    musicOn = true; // Initialize musicOn to true
}

void AudioManager::toggleMusic()
{
    // Toggle the music state
    if (musicOn)
    {
        backgroundMusic.pause();
        musicOn = false;
    }
    else
    {
        backgroundMusic.play();
        musicOn = true;
    }
}

void AudioManager::setVolume(float volume)
{
    backgroundMusic.setVolume(volume); // Volume range is 0 to 100
}

bool AudioManager::isMusicOn() const
{
    return musicOn;
}

float AudioManager::getVolume() const
{
    return backgroundMusic.getVolume();
}

// SFXManager Implementation
SFXManager::SFXManager()
{
    loadAllSounds();
    setVolume(volume);
}

void SFXManager::loadAllSounds()
{
    struct SFXData
    {
        std::string name;
        std::string filepath;
    };

    // List of sound effects with their names and file paths
    std::vector<SFXData> files = {
        {"opclick", "sfx/switch20.ogg"},
        {"onoff", "sfx/switch34.ogg"},
        {"plshoot", "sfx/laserSmall_000.ogg"},
        {"kcoll", "sfx/handleCoins.ogg"},
        {"chest", "sfx/metalLatch.ogg"},
        {"explode", "sfx/laserRetro_002.ogg"},
        {"bdefeat", "sfx/error_008.ogg"},
        {"bshoot", "sfx/laserSmall_001.ogg"},
        {"pldmg", "sfx/footstep_carpet_002.ogg"},
        {"enmdmg", "sfx/laserRetro_002.ogg"},
        {"respawn", "sfx/maximize_008.ogg"},
        {"bsdmg", "sfx/footstep_concrete_002.ogg"}};

    for (const auto &sfx : files)
    {
        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(sfx.filepath))
        {
            std::cerr << "Failed to load sound: " << sfx.filepath << "\n";
            continue;
        }
        buffers[sfx.name] = buffer;

        // Insert sound once and get iterator + insertion status:
        auto [it, inserted] = sounds.emplace(sfx.name, sf::Sound(buffers[sfx.name]));
        if (inserted)
        {
            it->second.setVolume(volume);
        }
    }
}

// Play sound by name
void SFXManager::playSound(const std::string &name)
{
    auto it = sounds.find(name);
    if (it != sounds.end())
    {
        it->second.play();
    }
    else
    {
        std::cerr << "Sound not found: " << name << "\n";
    }
}

void SFXManager::setVolume(float v)
{
    volume = v;
    for (auto &[_, sound] : sounds)
    {
        sound.setVolume(volume); // Set volume for each sound
    }
}

void SFXManager::playSoundWithVolume(const std::string &name, float volume)
{
    auto it = sounds.find(name);
    if (it != sounds.end())
    {
        it->second.setVolume(std::clamp(volume, 0.f, 100.f)); // Clamp to [0, 100]
        it->second.play();
    }
    else
    {
        std::cerr << "Sound not found: " << name << "\n";
    }
}

float SFXManager::getVolume() const
{
    return volume;
}
