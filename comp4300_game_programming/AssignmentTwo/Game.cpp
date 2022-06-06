#include "Game.h"
#include "Math.h"

#include <fstream>

#ifdef _WIN32
#include <Windows.h>
#endif

Game::Game(const std::string& configFile)
{
    #if defined(_MSC_VER)
    wchar_t path[FILENAME_MAX] = {0};
    GetModuleFileNameW(nullptr, path, FILENAME_MAX);
    m_gamePath = fs::path(path).parent_path();
    #else
    char path[FILENAME_MAX];
    ssize_t count = readlink("/proc/self/exe", path, FILENAME_MAX);
    m_gamePathh = fs::path(std::string(path, (count > 0) ? count : 0)).parent_path()
    #endif

    init(configFile);
}

void Game::run()
{
    while (m_running)
    {
        m_entities.update();

        if (!m_paused)
        {
            sLifeSpan();
            sSpawner();
            sMovement();
            sCollision();
        }
        sUserInput();
        sRender();

        //Increment the current frame
        ++m_currentFrame;
    }
}

void Game::init(const std::string& config)
{
    m_backgroundTexture.loadFromFile(m_gamePath.string() + "/" + "grid.png");
    m_backgroundSprite.setTexture(m_backgroundTexture);
    m_backgroundSprite.setPosition(sf::Vector2f(0, 0));

    std::fstream input(m_gamePath.string() + "/" + config);
    if (!input.is_open())
    {
        std::cout << "Failed to open: " << config << '\n';
        exit(-1); // NOLINT(concurrency-mt-unsafe)
    }

    std::string identifier{};
    while (input >> identifier)
    {
        if (identifier == "Window")
        {
            unsigned int width{};
            input >> width;

            unsigned int height{};
            input >> height;

            int frameLimit{};
            input >> frameLimit;

            int fullScreen{};
            input >> fullScreen;
            if (fullScreen == 0)
            {
                m_window.create(sf::VideoMode(width, height), "Geometry Wars", sf::Style::Close);
                m_window.setFramerateLimit(frameLimit);
            }
            else if (fullScreen == 1)
            {
                auto fullscreenMode = sf::VideoMode::getFullscreenModes(); // Detect available fullscreen modes
                m_window.create(fullscreenMode[0], "Geometry Wars", sf::Style::Fullscreen);
                m_window.setFramerateLimit(frameLimit);
            }
        }
        else if (identifier == "Font")
        {
            std::string filepath{};
            input >> filepath;
            if (!m_font.loadFromFile(filepath))
            {
                std::cerr << "Failed to load font. Filepath: " << filepath;
            }

            m_scoreText.setFont(m_font);

            int fontSize{};
            input >> fontSize;
            m_scoreText.setCharacterSize(fontSize);

            sf::Vector3<sf::Uint16> rgb{};
            input >> rgb.x;
            input >> rgb.y;
            input >> rgb.z;
            m_scoreText.setFillColor(
                sf::Color(
                    static_cast<sf::Uint8>(rgb.x),
                    static_cast<sf::Uint8>(rgb.y),
                    static_cast<sf::Uint8>(rgb.z)
                )
            );
        }
        else if (identifier == "Player")
        {
            int shapeRadius{};
            input >> shapeRadius;
            m_playerConfig.SR = shapeRadius;

            int collisionRadius{};
            input >> collisionRadius;
            m_playerConfig.CR = collisionRadius;

            float speed{};
            input >> speed;
            m_playerConfig.S = speed;

            sf::Vector3<sf::Uint16> fillColor{};
            input >> fillColor.x;
            input >> fillColor.y;
            input >> fillColor.z;
            m_playerConfig.FR = static_cast<sf::Uint8>(fillColor.x);
            m_playerConfig.FG = static_cast<sf::Uint8>(fillColor.y);
            m_playerConfig.FB = static_cast<sf::Uint8>(fillColor.z);

            sf::Vector3<sf::Uint16> outlineColor{};
            input >> outlineColor.x;
            input >> outlineColor.y;
            input >> outlineColor.z;
            m_playerConfig.OR = static_cast<sf::Uint8>(outlineColor.x);
            m_playerConfig.OG = static_cast<sf::Uint8>(outlineColor.y);
            m_playerConfig.OB = static_cast<sf::Uint8>(outlineColor.z);

            int outlineThickness{};
            input >> outlineThickness;
            m_playerConfig.OT = outlineThickness;

            int vertices{};
            input >> vertices;
            m_playerConfig.V = vertices;
        }
        else if (identifier == "Enemy")
        {
            int shapeRadius{};
            input >> shapeRadius;
            m_enemyConfig.SR = shapeRadius;

            int collisionRadius{};
            input >> collisionRadius;
            m_enemyConfig.CR = collisionRadius;

            float minSpeed{};
            input >> minSpeed;
            m_enemyConfig.SMIN = minSpeed;

            float maxSpeed{};
            input >> maxSpeed;
            m_enemyConfig.SMAX = maxSpeed;

            sf::Vector3<sf::Uint16> outlineColor{};
            input >> outlineColor.x;
            input >> outlineColor.y;
            input >> outlineColor.z;
            m_enemyConfig.OR = static_cast<sf::Uint8>(outlineColor.x);
            m_enemyConfig.OG = static_cast<sf::Uint8>(outlineColor.y);
            m_enemyConfig.OB = static_cast<sf::Uint8>(outlineColor.z);

            int outlineThickness{};
            input >> outlineThickness;
            m_enemyConfig.OT = outlineThickness;

            int minVertices{};
            input >> minVertices;
            m_enemyConfig.VMIN = minVertices;

            int maxVertices{};
            input >> maxVertices;
            m_enemyConfig.VMAX = maxVertices;

            int smallLifespan{};
            input >> smallLifespan;
            m_enemyConfig.L = smallLifespan;

            int spawnInterval{};
            input >> spawnInterval;
            m_enemyConfig.SI = spawnInterval;
        }
        else if (identifier == "Bullet")
        {
            int shapeRadius{};
            input >> shapeRadius;
            m_bulletConfig.SR = shapeRadius;

            int collisionRadius{};
            input >> collisionRadius;
            m_bulletConfig.CR = collisionRadius;

            float speed{};
            input >> speed;
            m_bulletConfig.S = speed;

            sf::Vector3<sf::Uint16> fillColor{};
            input >> fillColor.x;
            input >> fillColor.y;
            input >> fillColor.z;
            m_bulletConfig.FR = static_cast<sf::Uint8>(fillColor.x);
            m_bulletConfig.FG = static_cast<sf::Uint8>(fillColor.y);
            m_bulletConfig.FB = static_cast<sf::Uint8>(fillColor.z);

            sf::Vector3<sf::Uint16> outlineColor{};
            input >> outlineColor.x;
            input >> outlineColor.y;
            input >> outlineColor.z;
            m_bulletConfig.OR = static_cast<sf::Uint8>(outlineColor.x);
            m_bulletConfig.OG = static_cast<sf::Uint8>(outlineColor.y);
            m_bulletConfig.OB = static_cast<sf::Uint8>(outlineColor.z);

            int outlineThickness{};
            input >> outlineThickness;
            m_bulletConfig.OT = outlineThickness;

            int shapeVertices{};
            input >> shapeVertices;
            m_bulletConfig.V = shapeVertices;

            int lifespan{};
            input >> lifespan;
            m_bulletConfig.L = lifespan;

            int specialBulletAmount{};
            input >> specialBulletAmount;
            m_bulletConfig.SB = specialBulletAmount;
        }
    }

    m_scoreText.setPosition(0, 0);
    m_scoreText.setString(std::to_string(m_score));

    spawnPlayer();
}

void Game::setPaused(const bool paused)
{
    m_paused = paused;
}

void Game::sMovement()
{
    Vec2 playerVelocity;
    if (m_player->cInput->left)
    {
        playerVelocity.x -= m_playerConfig.S;
    }
    if (m_player->cInput->right)
    {
        playerVelocity.x += m_playerConfig.S;
    }
    if (m_player->cInput->up)
    {
        playerVelocity.y -= m_playerConfig.S;
    }
    if (m_player->cInput->down)
    {
        playerVelocity.y += m_playerConfig.S;
    }

    for (const auto& entity : m_entities.getEntities())
    {
        if (entity->tag() == EntityTags::Player)
        {
            m_player->cTransform->pos += playerVelocity;
            entity->cTransform->angle += 2.0f;
            entity->cShape->circle.setRotation(entity->cTransform->angle);
        }
        else if (entity->cTransform)
        {
            entity->cTransform->pos += entity->cTransform->velocity;
            entity->cTransform->angle += 2.0f;
            entity->cShape->circle.setRotation(entity->cTransform->angle);
        }
    }
}

void Game::sUserInput()
{
    sf::Event event = {};
    while (m_window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            {
                m_running = false;
                break;
            }
        case sf::Event::KeyPressed:
            {
                if (event.key.code == sf::Keyboard::Escape)
                    m_running = false;
                else if (event.key.code == sf::Keyboard::P)
                    m_paused ? setPaused(false) : setPaused(true);
                else if (event.key.code == sf::Keyboard::A)
                    m_player->cInput->left = true;
                else if (event.key.code == sf::Keyboard::D)
                    m_player->cInput->right = true;
                else if (event.key.code == sf::Keyboard::W)
                    m_player->cInput->up = true;
                else if (event.key.code == sf::Keyboard::S)
                    m_player->cInput->down = true;

                break;
            }
        case sf::Event::KeyReleased:
            {
                if (event.key.code == sf::Keyboard::A)
                    m_player->cInput->left = false;
                else if (event.key.code == sf::Keyboard::D)
                    m_player->cInput->right = false;
                else if (event.key.code == sf::Keyboard::W)
                    m_player->cInput->up = false;
                else if (event.key.code == sf::Keyboard::S)
                    m_player->cInput->down = false;
                break;
            }
        case sf::Event::MouseButtonPressed:
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (m_player->cInput->leftMouse == false)
                        m_player->cInput->leftMouse = true;
                }
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    if (m_player->cInput->rightMouse == false)
                        m_player->cInput->rightMouse = true;
                }
                break;
            }
        case sf::Event::MouseButtonReleased:
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                    m_player->cInput->leftMouse = false;
                break;
            }

        default:
            break;
        }
    }
}

void Game::sLifeSpan()
{
    for (const auto& entity : m_entities.getEntities())
    {
        if (!entity->cLifespan)
            continue;

        if (entity->cLifespan->remaining > 0)
            --entity->cLifespan->remaining;

        if (entity->isActive() && entity->cLifespan->remaining > 0)
        {
            const float alphaMultiplier = static_cast<float>(entity->cLifespan->remaining) / static_cast<float>(entity->cLifespan->total);

            const auto fillColor   = entity->cShape->circle.getFillColor();
            sf::Color newFillColor = {fillColor.r, fillColor.g, fillColor.b, static_cast<sf::Uint8>(255 * alphaMultiplier)};
            entity->cShape->circle.setFillColor(newFillColor);

            const auto outlineColor   = entity->cShape->circle.getOutlineColor();
            sf::Color newOutlineColor = {outlineColor.r, outlineColor.g, outlineColor.b, static_cast<sf::Uint8>(255 * alphaMultiplier)};
            entity->cShape->circle.setOutlineColor(newOutlineColor);
        }
        else if (entity->cLifespan->remaining <= 0)
        {
            entity->destroy();
        }
    }
}

void Game::sRender()
{
    m_window.clear();
    m_window.draw(m_backgroundSprite);

    for (const auto& entity : m_entities.getEntities())
    {
        entity->cShape->circle.setPosition(entity->cTransform->pos.x, entity->cTransform->pos.y);
        m_window.draw(entity->cShape->circle);
    }
    m_window.draw(m_scoreText);

    m_window.display();
}

void Game::sSpawner()
{
    // Enemies
    if ((m_currentFrame - m_lastEnemySpawnTime) >= m_enemyConfig.SI)
    {
        spawnEnemy();
    }

    // Bullets
    if (m_player->cInput->leftMouse == true)
    {
        const Vec2 mousePos = {
            static_cast<float>(sf::Mouse::getPosition(m_window).x),
            static_cast<float>(sf::Mouse::getPosition(m_window).y)
        };
        spawnBullet(m_player, mousePos);
        m_player->cInput->leftMouse = false;
    }
    // Special ability
    if (m_player->cInput->rightMouse == true)
    {
        spawnSpecialWeapon(m_player);
        m_player->cInput->rightMouse = false;
    }
}

void Game::sCollision()
{
    // Player Collisions
    for (const auto& player : m_entities.getEntities(EntityTags::Player))
    {
        // Player and Enemy Collision
        for (const auto& enemy : m_entities.getEntities(EntityTags::Enemy))
        {
            const Vec2 distance = {
                enemy->cTransform->pos.x - player->cTransform->pos.x,
                enemy->cTransform->pos.y - player->cTransform->pos.y
            };

            const float collisionRadiusNorm = (player->cCollision->radius + enemy->cCollision->radius) * (player->cCollision->radius + enemy
                ->cCollision->radius);
            const float distanceNorm = distance.x * distance.x + distance.y * distance.y;

            if (distanceNorm < collisionRadiusNorm)
            {
                if (player->isActive()) // Not allow 2 players
                {
                    m_score = 0;
                    m_scoreText.setString(std::to_string(m_score));
                    enemy->destroy();
                    player->destroy();
                    spawnPlayer();
                }
            }
        }

        // Player and Enemy Fragment Collision
        for (const auto& enemy : m_entities.getEntities(EntityTags::EnemyFragment))
        {
            const Vec2 distance = {
                enemy->cTransform->pos.x - player->cTransform->pos.x,
                enemy->cTransform->pos.y - player->cTransform->pos.y
            };

            const float collisionRadiusNorm = (player->cCollision->radius + enemy->cCollision->radius) * (player->cCollision->radius + enemy
                ->cCollision->radius);
            const float distanceNorm = distance.x * distance.x + distance.y * distance.y;

            if (distanceNorm < collisionRadiusNorm)
            {
                if (player->isActive())
                {
                    m_score /= 2;
                    m_scoreText.setString(std::to_string(m_score));

                    player->destroy();
                    enemy->destroy();
                    spawnPlayer();
                }
            }
        }
    }

    // Bullet Collisions
    for (const auto& bullet : m_entities.getEntities(EntityTags::Bullet))
    {
        // Bullet and Enemy collision
        for (const auto& enemy : m_entities.getEntities(EntityTags::Enemy))
        {
            const Vec2 distance = {
                enemy->cTransform->pos.x - bullet->cTransform->pos.x,
                enemy->cTransform->pos.y - bullet->cTransform->pos.y
            };

            const float collisionRadiusNorm = (bullet->cCollision->radius + enemy->cCollision->radius) * (bullet->cCollision->radius + enemy
                ->cCollision->radius);
            const float distanceNorm = distance.x * distance.x + distance.y * distance.y;

            if (distanceNorm < collisionRadiusNorm)
            {
                m_score += enemy->cScore->score;
                m_scoreText.setString(std::to_string(m_score));

                spawnSmallEnemies(enemy);
                bullet->destroy();
                enemy->destroy();

                break;
            }
        }

        // Bullet and Small Enemy collision
        for (const auto& enemy : m_entities.getEntities(EntityTags::EnemyFragment))
        {
            const Vec2 distance = {
                enemy->cTransform->pos.x - bullet->cTransform->pos.x,
                enemy->cTransform->pos.y - bullet->cTransform->pos.y
            };

            const float collisionRadiusNorm = (bullet->cCollision->radius + enemy->cCollision->radius) * (bullet->cCollision->radius + enemy
                ->cCollision->radius);
            const float distanceNorm = distance.x * distance.x + distance.y * distance.y;

            if (distanceNorm < collisionRadiusNorm)
            {
                m_score += enemy->cScore->score;
                m_scoreText.setString(std::to_string(m_score));

                bullet->destroy();
                enemy->destroy();

                break;
            }
        }
    }

    // Player <=> Walls
    for (auto& entity : m_entities.getEntities(EntityTags::Player))
    {
        if (entity->cTransform->pos.x + static_cast<float>(m_playerConfig.CR) > static_cast<float>(m_window.getSize().x))
        {
            entity->cTransform->pos.x -= m_playerConfig.S;
        }
        else if (entity->cTransform->pos.x - static_cast<float>(m_playerConfig.CR) < 0.0f)
        {
            entity->cTransform->pos.x += m_playerConfig.S;
        }
        if (entity->cTransform->pos.y + static_cast<float>(m_playerConfig.CR) > static_cast<float>(m_window.getSize().y))
        {
            entity->cTransform->pos.y -= m_playerConfig.S;
        }
        else if (entity->cTransform->pos.y - static_cast<float>(m_playerConfig.CR) < 0.0f)
        {
            entity->cTransform->pos.y += m_playerConfig.S;
        }
    }

    // Bullet, Enemy, EnemyFragment <=> Walls
    for (const auto& entity : m_entities.getEntities())
    {
        if (entity->tag() == EntityTags::Bullet || entity->tag() == EntityTags::Enemy || entity->tag() == EntityTags::EnemyFragment)
        {
            if (entity->cTransform->pos.x + entity->cCollision->radius > static_cast<float>(m_window.getSize().x))
            {
                entity->cTransform->velocity.x *= -1;
            }
            else if (entity->cTransform->pos.x - entity->cCollision->radius < 0.0f)
            {
                entity->cTransform->velocity.x *= -1;
            }
            if (entity->cTransform->pos.y + entity->cCollision->radius > static_cast<float>(m_window.getSize().y))
            {
                entity->cTransform->velocity.y *= -1;
            }
            else if (entity->cTransform->pos.y - entity->cCollision->radius < 0.0f)
            {
                entity->cTransform->velocity.y *= -1;
            }
        }
    }
}

void Game::spawnPlayer()
{
    const auto playerEntity = m_entities.addEntity(EntityTags::Player);

    Vec2 middleWindowPos =
    {
        static_cast<float>(m_window.getSize().x) * 0.5f,
        static_cast<float>(m_window.getSize().y) * 0.5f
    };

    playerEntity->cTransform = std::make_shared<CTransform>(middleWindowPos, Vec2(0.0f, 0.0f), 0.0f);

    //Creates the entities shape
    playerEntity->cShape = std::make_shared<CShape>(
        static_cast<float>(m_playerConfig.SR), m_playerConfig.V,
        sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
        sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB),
        static_cast<float>(m_playerConfig.OT));

    playerEntity->cCollision = std::make_shared<CCollision>(static_cast<float>(m_playerConfig.CR));
    playerEntity->cInput     = std::make_shared<CInput>();
    m_player                 = playerEntity;
}

void Game::spawnEnemy()
{
    const auto enemy   = m_entities.addEntity(EntityTags::Enemy);
    const auto minPosX = static_cast<float>(m_enemyConfig.CR);
    const auto maxPosX = static_cast<float>(m_window.getSize().x - m_enemyConfig.CR);
    const auto minPosY = static_cast<float>(m_enemyConfig.CR);
    const auto maxPosY = static_cast<float>(m_window.getSize().y - m_enemyConfig.CR);

    Vec2 randPos = {
        minPosX + Math::randomNumber<float>(0, maxPosX - minPosX + 1),
        minPosY + Math::randomNumber<float>(0, maxPosY - minPosY + 1)
    };

    float randSpeed = m_enemyConfig.SMIN + Math::randomNumber<float>(0, m_enemyConfig.SMAX - m_enemyConfig.SMIN + 1);

    while (randSpeed == 0.0f)
    {
        randSpeed = m_enemyConfig.SMIN + Math::randomNumber<float>(0, m_enemyConfig.SMAX - m_enemyConfig.SMIN + 1);
    }

    //Gives the entity the random position on the screen
    enemy->cTransform = std::make_shared<CTransform>(randPos, Vec2(randSpeed, randSpeed), 0.0f);

    //Give the enemy a score of 100
    enemy->cScore = std::make_shared<CScore>(100);

    int vertices = m_enemyConfig.VMIN + Math::randomNumber<int>(0, m_enemyConfig.VMAX - m_enemyConfig.VMIN + 1);

    sf::Color fillColor
    {
        static_cast<sf::Uint8>(Math::randomNumber<int>(0, 1 + 255)),
        static_cast<sf::Uint8>(Math::randomNumber<int>(0, 1 + 255)),
        static_cast<sf::Uint8>(Math::randomNumber<int>(0, 1 + 255))
    };

    sf::Color outlineColor     = {m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB};
    const int outlineThickness = m_enemyConfig.OT;

    enemy->cShape =
        std::make_shared<CShape>(
            static_cast<float>(m_enemyConfig.SR), vertices, fillColor, outlineColor, static_cast<float>(outlineThickness));

    enemy->cCollision = std::make_shared<CCollision>(static_cast<float>(m_enemyConfig.CR));

    //Record when the most recent enemy was spawned
    m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(const std::shared_ptr<Entity>& parent)
{
    //Spawn a number of small enemies equal to the number of vertices of the original
    const size_t vertices = parent->cShape->circle.getPointCount();

    Vec2 parentPos                 = {parent->cTransform->pos.x, parent->cTransform->pos.y};
    const Vec2 normalizedParentPos = Vec2::normalize(parentPos);

    //Set each enemy to the same color as the original, half the size
    sf::Color parentFill            = parent->cShape->circle.getFillColor();
    sf::Color parentOutline         = parent->cShape->circle.getOutlineColor();
    float parentTickness            = parent->cShape->circle.getOutlineThickness();
    float smallEnemyRadius          = parent->cShape->circle.getRadius() * 0.5f;
    float smallEnemyCollisionRadius = parent->cCollision->radius * 0.5f;

    float angle = 0.0f;
    // std::cout << "Parent: " << parentPos << " Parent normalized: " << normalizedParentPos << "\n";

    for (size_t i = 0; i < vertices; ++i)
    {
        const auto enemyFragment = m_entities.addEntity(EntityTags::EnemyFragment);
        enemyFragment->cScore    = std::make_shared<CScore>(parent->cScore->score * 2);
        enemyFragment->cShape    = std::make_shared<CShape>(smallEnemyRadius, static_cast<int>(vertices), parentFill, parentOutline,
                                                            parentTickness);
        enemyFragment->cCollision = std::make_shared<CCollision>(smallEnemyCollisionRadius);
        enemyFragment->cLifespan  = std::make_shared<CLifeSpan>(m_enemyConfig.L);

        // Calculate the velocity
        const float radians = angle * Math::PI / 180.0f;
        const Vec2 velocity
        {
            std::cos(radians) * normalizedParentPos.x + std::sin(radians) * normalizedParentPos.y,
            std::sin(radians) * normalizedParentPos.x - std::cos(radians) * normalizedParentPos.y,
        };

        // Normalize velocity vector
        const float lenght = velocity.lenght();
        const Vec2 normalizedVelocity = {velocity.x / lenght, velocity.y / lenght};
        Vec2 newVelocity = {normalizedVelocity.x * parent->cTransform->velocity.x, normalizedVelocity.y * parent->cTransform->velocity.y};

        enemyFragment->cTransform = std::make_shared<CTransform>(parentPos, newVelocity, 0.0f);

        angle += 360.0f / static_cast<float>(vertices);
    }
}

void Game::spawnBullet(const std::shared_ptr<Entity>& shooter, const Vec2& mousePos)
{
    const auto bullet = m_entities.addEntity(EntityTags::Bullet);

    bullet->cCollision = std::make_shared<CCollision>(static_cast<float>(m_bulletConfig.CR));
    bullet->cLifespan  = std::make_shared<CLifeSpan>(m_bulletConfig.L);
    bullet->cShape     =
        std::make_shared<CShape>(
            static_cast<float>(m_bulletConfig.SR), m_bulletConfig.V,
            sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
            sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB),
            static_cast<float>(m_bulletConfig.OT));

    Vec2 difference = {mousePos.x - shooter->cTransform->pos.x, mousePos.y - shooter->cTransform->pos.y};
    difference.normalize();
    Vec2 velocity{m_bulletConfig.S * difference.x, m_bulletConfig.S * difference.y};

    bullet->cTransform = std::make_shared<CTransform>(shooter->cTransform->pos, velocity, 0.0f);
}

void Game::spawnSpecialWeapon([[maybe_unused]] std::shared_ptr<Entity> entity)
{
    float angle = 0.0f;

    for (int j = 0; j < m_bulletConfig.SB; ++j)
    {
        const auto bullet = m_entities.addEntity(EntityTags::Bullet);
        bullet->cShape    =
            std::make_shared<CShape>(
                static_cast<float>(m_bulletConfig.SR), m_bulletConfig.V,
                sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
                sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB),
                static_cast<float>(m_bulletConfig.OT));

        bullet->cCollision = std::make_shared<CCollision>(static_cast<float>(m_bulletConfig.CR));
        bullet->cLifespan  = std::make_shared<CLifeSpan>(m_bulletConfig.L);

        // Calculate the velocity
        const Vec2 normalizedPos = Vec2::normalize(m_player->cTransform->pos);
        const float radians      = angle * Math::PI / 180.0f;
        Vec2 velocity            = {
            std::cos(radians) * normalizedPos.x + std::sin(radians) * normalizedPos.y,
            std::sin(radians) * normalizedPos.x - std::cos(radians) * normalizedPos.y,
        };

        // Normalize velocity vector
        const float lenght            = velocity.lenght();
        const Vec2 normalizedVelocity = {velocity.x / lenght, velocity.y / lenght};
        Vec2 newVelocity              = {normalizedVelocity.x * m_bulletConfig.S, normalizedVelocity.y * m_bulletConfig.S};

        bullet->cTransform = std::make_shared<CTransform>(m_player->cTransform->pos, newVelocity, 0.0f);

        angle += 360.0f / static_cast<float>(m_bulletConfig.SB);
    }
}
