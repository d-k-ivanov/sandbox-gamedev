#pragma once

#include "EntityManager.h"
#include "Entity.h"

#include <filesystem>
namespace fs = std::filesystem;

struct PlayerConfig
{
    int SR, CR, OT, V;                // TODO: Convert to float
    sf::Uint8 FR, FG, FB, OR, OG, OB; // Colors
    float S;
};

struct EnemyConfig
{
    int SR, CR, OT, VMIN, VMAX, L, SI; // TODO: Convert to float
    sf::Uint8 OR, OG, OB;              // Colors
    float SMIN, SMAX;
};

struct BulletConfig
{
    int SR, CR, OT, V, L, SB;         // TODO: Convert to float
    sf::Uint8 FR, FG, FB, OR, OG, OB; // Colors
    float S;
};

class Game
{
private:
    fs::path m_gamePath;
    sf::RenderWindow m_window;
    EntityManager m_entities;
    sf::Font m_font;
    sf::Text m_scoreText;
    PlayerConfig m_playerConfig = {};
    EnemyConfig m_enemyConfig   = {};
    BulletConfig m_bulletConfig = {};

    int m_score              = 0;
    int m_currentFrame       = 0;
    int m_lastEnemySpawnTime = 0;
    bool m_paused            = false;
    bool m_running           = true;

    sf::Texture m_backgroundTexture;
    sf::Sprite m_backgroundSprite;

    std::shared_ptr<Entity> m_player;

    void init(const std::string& config);
    void setPaused(bool paused);

    void sMovement();
    void sUserInput();
    void sLifeSpan();
    void sRender();
    void sSpawner();
    void sCollision();

    void spawnPlayer();
    void spawnEnemy();
    void spawnEnemyFragments(const std::shared_ptr<Entity>& parent);
    void spawnBullet(const std::shared_ptr<Entity>& shooter, const Vec2& mousePos);
    void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

public:
    explicit Game(const std::string& configFile);
    void run();
};
