#pragma once

#include "Components.h"

enum class EntityTags
{
    None,
    Player,
    Enemy,
    EnemyFragment,
    Bullet
};

class Entity
{
    friend class EntityManager;

    bool m_active    = true;
    size_t m_id      = 0;
    EntityTags m_tag = EntityTags::None;

    Entity(size_t id, EntityTags tag);

public:
    std::shared_ptr<CCollision> cCollision;
    std::shared_ptr<CInput> cInput;
    std::shared_ptr<CLifeSpan> cLifespan;
    std::shared_ptr<CScore> cScore;
    std::shared_ptr<CShape> cShape;
    std::shared_ptr<CTransform> cTransform;

public:
    [[nodiscard]] bool isActive() const;
    [[nodiscard]] EntityTags tag() const;
    [[nodiscard]] size_t id() const;
    void destroy();
};
