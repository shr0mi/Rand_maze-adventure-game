#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <bits/stdc++.h>
#include "menu.hpp"
#include "options.hpp"
#include "credits.hpp"
#include "view_manager.hpp"
#include "audio.hpp"
#include "mapgen.h"
#include "player.h"
#include "enemy.hpp"
#include "bsp_algorithm.h"
#include "timer.h"
#include "pause.hpp"
#include "leaderboard.hpp"
#include "controls.hpp"


std::vector<std::shared_ptr<BaseEnemy>> enemies;
sf::Texture texture;
std::vector<Key> keys;
sf::Vector2f pos;
sf::Vector2f chest_pos, boss_pos;

AudioManager audioManager;
SFXManager sfx;

void OpenLeaderboard(GameTimer &gameTimer);

std::vector<std::vector<int>> generateCollisionMap(std::vector<std::vector<int>> &level, sf::Texture &tex)
{
    enemies.clear();
    keys.clear();
    std::vector<std::vector<int>> collisionMap(level.size(), std::vector<int>(level[0].size(), 0));
    for (int i = 0; i < level.size(); i++)
    {
        for (int j = 0; j < level[i].size(); j++)
        {
            if ((50 <= level[i][j] && level[i][j] <= 55) || level[i][j] == 12 || level[i][j] == 638)
            {
                collisionMap[i][j] = 1;
            }
            else
            {
                collisionMap[i][j] = 0;
            }

            if (level[i][j] == -1)
            {
                enemies.push_back(std::make_shared<ShooterEnemy>(texture, j * 16.f, i * 16.f));
            }
            else if (level[i][j] == -2)
            {
                enemies.push_back(std::make_shared<TurretEnemy>(texture, j * 16.f, i * 16.f));
            }
            else if (level[i][j] == -3)
            {
                enemies.push_back(std::make_shared<ExploderEnemy>(texture, j * 16.f, i * 16.f));
            }
            else if (level[i][j] == -4)
            {
                pos.x = j * 16.f;
                pos.y = i * 16.f;
                cout << pos.x << " " << pos.y << endl;
            }
            else if (level[i][j] == -5)
            {
                Key k({j * 16.f, i * 16.f}, tex);
                keys.push_back(k);
            }
            else if (level[i][j] == -6)
            {
                chest_pos = {j * 16.f, i * 16.f};
                boss_pos = {j * 16.f, i * 16.f};
            }
        }
    }

    return collisionMap;
}

void runGame(sf::RenderWindow &window)
{
    GameTimer gameTimer;
    PauseMenu pauseMenu;
    pauseMenu.syncWithAudio(audioManager);
    window.setMouseCursorVisible(false);


    sf::Clock clock;
    bool isPaused = false; // Variable for pausing game
    bool firstTime = true; // Variable to check if leaderboard is opened for the first time
    bool isOver = false;   // Variable to check if game is over

    if (!texture.loadFromFile("colored-transparent.png"))
    {
        return; // Failed to load texture
    }
    // Level Vector
    // Trees -> 50, 51, 52, 53, 54, 55
    // Walls -> 19, 20, 21, 68, *69, 70, 117, 118, 119
    // Floor -> 2, 3, 4, 5

    BSP_algorithm bsp;
    std::vector<std::vector<int>> level = bsp.generate_bsp_map(100, 100);
    std::vector<std::vector<int>> collisionMap = generateCollisionMap(level, texture);

    // Test
    GameInfo gameinfo(collisionMap, texture);
    Chest chest(gameinfo);
    chest.setpos(chest_pos);
    Boss boss(gameinfo);
    boss.setpos(boss_pos);

    Player player(texture, 3, 24, collisionMap, gameinfo);
    player.setPos(pos);
    Crosshair crosshair(texture, 14, 21);
    std::vector<Bullet> playerBullets;
    std::vector<EnemyBullet> enemyBullets;

    TileMap map;
    if (!map.load("colored-transparent.png", sf::Vector2u(832, 373), level, 16, 16, 1))
    {
        std::cerr << "Failed to load tilemap!" << std::endl;
        return;
    }

    sf::Texture winTexture;
    if (!winTexture.loadFromFile("assets/youWin.png"))
    {
        std::cerr << "Failed to load win texture!" << std::endl;
        return;
    }
    sf::Sprite winSprite(winTexture);
    winSprite.setPosition({120, 300});
    winSprite.setScale({0.4f, 0.4f});

    sf::Font font;
    if (!font.openFromFile("assets/arial.ttf"))
    {
        std::cerr << "Failed to load font!" << std::endl;
        return;
    }
    sf::Text timeText(font);
    timeText.setCharacterSize(50);
    timeText.setFillColor(sf::Color::Yellow);
    timeText.setPosition({300, 500}); // Adjust to position text below the sprite

    // --- Added Key Counter Text ---
    // sf::Text keyCounterText(font,"Keys: 0 / 0", 30);
    // keyCounterText.setFont(font);
    // keyCounterText.setCharacterSize(30);
    // keyCounterText.setFillColor(sf::Color::Yellow);
    // keyCounterText.setPosition({200.f, 10.f});
    // -----------------------------

    while (window.isOpen())
    {
        while (std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            // Press Esc to Pause
            if (event->is<sf::Event::KeyPressed>())
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                {
                    isPaused = !isPaused;
                }
            }

            if (isPaused)
            {
                pauseMenu.handleEvent(*event, window, audioManager);
                window.setMouseCursorVisible(true);


                if (event->is<sf::Event::MouseButtonPressed>())
                {
                    sfx.playSound("opclick");
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    if (pauseMenu.isResumeClicked(mousePos))
                    {
                        sfx.playSound("opclick");
                        isPaused = false;
                    }
                    else if (pauseMenu.isMenuClicked(mousePos))
                    {
                        sfx.playSound("opclick");
                        return; // Back to main menu
                    }
                }
            }
            
            if(!isPaused){window.setMouseCursorVisible(false);} 

            // If Game is Over then write it to Leaderboard
            if (chest.isOpened() && firstTime)
            {
                OpenLeaderboard(gameTimer);
                firstTime = false; // Ensure leaderboard is opened only once
                isOver = true;     // Set game over state
            }

            // If you want to end the game instantly- just click X button!
            // if (event->is<sf::Event::KeyPressed>())
            // {
            //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X))
            //     {
            //         OpenLeaderboard(gameTimer);
            //         firstTime = false; // Ensure leaderboard is opened only once
            //         isOver = true;
            //     }
            // }

            if (isOver)
            {
                // Game is over
                // And pause menu isn't accessible

                isPaused = false; // Disable pause when game is over

                int totalSeconds = gameTimer.get_minutes() * 60 + gameTimer.get_seconds();
                std::stringstream ss;
                ss << "Your Time: " << gameTimer.get_minutes() << " m " << gameTimer.get_seconds() << " s";
                timeText.setString(ss.str()); // Set time text

                if (event->is<sf::Event::MouseButtonPressed>())
                {
                    // Click anywhere to return to menu
                    return;
                }
            }
        }

        // Player
        float dt = clock.restart().asSeconds();

        // Pause
        gameTimer.update(dt, isPaused, isOver);

        if (!isPaused && !isOver) // Only update game logic if not paused or over
        {
            player.cheatlook(window, dt); // Handle cheat mode camera

            for (auto &enemy : enemies)
            {
                // Dynamic cast to access ExploderEnemy update method with collision
                if (auto *exploder = dynamic_cast<ExploderEnemy *>(enemy.get()))
                {
                    exploder->update(dt, player.getPosition(), enemyBullets, collisionMap);
                }
                else
                {
                    enemy->update(dt, player.getPosition(), enemyBullets);
                }
            }

            // Update enemy bullets
            for (auto it = enemyBullets.begin(); it != enemyBullets.end();)
            {
                it->update(dt, collisionMap); // Move and check for collisions
                if (!it->isAlive)
                {
                    it = enemyBullets.erase(it); // remove bullet
                }
                else
                {
                    ++it;
                }
            }

            // Enemy Damage
            for (auto it = playerBullets.begin(); it != playerBullets.end();)
            {
                bool hit = false; // Flag to check if bullet hit an enemy
                for (auto &enemy : enemies)
                {
                    if (enemy->isAlive() && it->getBounds().findIntersection(enemy->getBounds()).has_value())
                    {
                        sfx.playSoundWithVolume("enmdmg",10);
                        enemy->takeDamage(1); // Adjust damage if needed
                        hit = true;
                        break;
                    }
                }

                if (hit)
                {
                    it = playerBullets.erase(it); // remove bullet
                }
                else
                {
                    ++it;
                }
            }

            enemies.erase(
                std::remove_if(enemies.begin(), enemies.end(),
                               [](const std::shared_ptr<BaseEnemy> &e)
                               {
                                   return !e->isAlive();
                               }),
                enemies.end());

            handleShooting(playerBullets, texture, player.getPosition(), window, gameinfo);
            updateBullets(playerBullets, dt, window, gameinfo);

            player.update(dt, enemyBullets, window, enemies, pos, boss.getBounds(), chest.isOpened());
            boss.update(dt, window, playerBullets, enemyBullets,player.getPosition());
            handleKeyChestInteraction(keys, chest, player.getPosition(), gameinfo);
            crosshair.update(window);

            // --- Update key counter string here ---
            // int collectedCount = 0;
            // for (const auto &key : keys)
            // {
            //     if (key.isCollected())
            //         collectedCount++;
            // }
            // std::stringstream ss;
            // ss << "Keys: " << collectedCount << " / " << keys.size();
            // keyCounterText.setString(ss.str());
            // --------------------------------------
            // --- CHANGED: Count collected keys and update timer ---
            int collectedKeyCount = 0;
            for (const auto &key : keys)
            {
                if (key.isCollected())
                    collectedKeyCount++;
            }
            gameTimer.setKeyCount(collectedKeyCount); // <== ADDED
            // ------------------------------------------------------
        }

        window.clear();
        window.setView(player.getCurrentView());
        window.draw(map);
        player.draw(window);
        crosshair.draw(window);
        gameTimer.draw(window); // Draw Timer

        for (auto &bullet : playerBullets)
            bullet.draw(window);

        for (auto &enemy : enemies)
            enemy->draw(window);

        for (auto &eBullet : enemyBullets)
            window.draw(eBullet.shape); // enemy bullets are just CircleShape

        for (auto &ky : keys)
            ky.draw(window);

        chest.draw(window);
        boss.draw(window);

        // Draw Pause Menu
        if (isPaused)
        {
            window.setView(window.getDefaultView());
            pauseMenu.draw(window, audioManager);
        }

        // Draw Win Screen
        if (isOver)
        {
            window.setView(window.getDefaultView()); // Reset view for overlay
            window.draw(winSprite);
            window.draw(timeText);

            // Draw key count on win screen as well
            // window.draw(keyCounterText);
            window.display();
            continue; // Skip remaining rendering
        }
        // Draw key count text on top of everything using default view
        //         sf::View currentView = window.getView();
        //         sf::Vector2f center = currentView.getCenter();
        //         //sf::Vector2f size = currentView.getSize();

        // // Position keyCounterText at top-left of the view (some offset)
        //         keyCounterText.setPosition(sf::Vector2f(center.x / 2 + 10.f, center.y/ 2 + 10.f));

        // Then draw without changing view
        // window.draw(keyCounterText);

        window.display();
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({960, 540}), "Main Menu", sf::Style::None);
    window.setFramerateLimit(60);

    ViewManager viewManager;
    MenuScreen menu;
    OptionsScreen options(audioManager);
    LeaderboardScreen leaderboard;
    ControlsScreen controls;
    CreditsScreen credits;

    Scene scene = Scene::Menu;

    while (window.isOpen())
    {
        while (std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (scene == Scene::Options)
                options.handleEvent(*event, window, audioManager);

            if (event->is<sf::Event::MouseButtonPressed>())
            {
                sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                sfx.playSound("opclick");
                
                if (scene == Scene::Menu)
                {
                    if (menu.playClick(pos))
                    {
                        // Switch to game
                        window.create(sf::VideoMode({1000, 1000}), "Main Game", sf::Style::None);
                        window.setView(sf::View(sf::FloatRect({0, 0}, {1000, 1000}))); // Fix view
                        runGame(window);                                               // run game loop

                        // After return
                        scene = Scene::Menu;
                        window.create(sf::VideoMode({960, 540}), "Main Menu", sf::Style::None);
                    }
                    else if (menu.optionClick(pos))
                    {
                        scene = Scene::Options;
                        viewManager.setView(Scene::Options);
                        options.syncWithAudio(audioManager);
                    }
                    else if (menu.boardClick(pos))
                    {
                        scene = Scene::Leaderboard;
                        viewManager.setView(Scene::Leaderboard);
                        options.syncWithAudio(audioManager);

                        leaderboard.reload(); // RELOAD SCORE DATA
                    }
                    else if (menu.controlClick(pos))
                    {
                        scene = Scene::Controls;
                        viewManager.setView(Scene::Controls);
                        options.syncWithAudio(audioManager);
                    }
                    else if (menu.exitClick(pos))
                    {
                        window.close();
                    }
                }
                else if (scene == Scene::Options)
                {
                    options.syncWithAudio(audioManager);

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
                else if (scene == Scene::Leaderboard && leaderboard.backClicked(pos))
                {
                    scene = Scene::Menu;
                    viewManager.setView(Scene::Menu);
                }
                else if (scene == Scene::Controls && controls.backClicked(pos))
                {
                    scene = Scene::Menu;
                    viewManager.setView(Scene::Menu);
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
        else if (scene == Scene::Leaderboard)
        {
            leaderboard.draw(window);
        }
        else if (scene == Scene::Controls)
        {
            controls.draw(window);
        }
        else if (scene == Scene::Credits)
        {
            credits.draw(window);
        }

        window.display();
    }

    return 0;
}

void OpenLeaderboard(GameTimer &gameTimer)
{
    cout << "You Win! Your Record: " << gameTimer.get_minutes() << " : " << gameTimer.get_seconds() << endl;

    vector<int> top_seven_scores;
    FILE *fptr;
    fptr = fopen("scoreboard.dat", "rb");
    int x;
    while (fread(&x, sizeof(int), 1, fptr) == 1)
    {
        top_seven_scores.push_back(x);
    }
    fclose(fptr);

    int current_score = gameTimer.get_minutes() * 60 + gameTimer.get_seconds();
    top_seven_scores.push_back(current_score);
    sort(top_seven_scores.begin(), top_seven_scores.end());

    fptr = fopen("scoreboard.dat", "wb");
    for (int i = 0; i < min(7, (int)top_seven_scores.size()); i++)
    {
        fwrite(&top_seven_scores[i], sizeof(int), 1, fptr);
        cout << top_seven_scores[i] << endl;
    }
    fclose(fptr);
}
