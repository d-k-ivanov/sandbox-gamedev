#include "EntityManager.h"

#include <ranges>

void EntityManager::removeDeadEntities(EntityVec& vec) const
{
    std::erase_if(vec, [](auto& entity) { return !entity->isActive(); });
}


void EntityManager::update()
{
    for (auto& entity : m_toAdd)
    {
        m_entities.push_back(entity);
        m_entityMap[entity->tag()].push_back(entity);
    }

    // remove dead entities
    removeDeadEntities(m_entities);
    for (auto& entityVec : m_entityMap | std::views::values)
    {
        removeDeadEntities(entityVec);
    }
    m_toAdd.clear();
}

std::shared_ptr<Entity> EntityManager::addEntity(const EntityTags tag)
{
    auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
    m_toAdd.push_back(entity);
    return entity;
}

const EntityVec& EntityManager::getEntities()
{
    return m_entities;
}

const EntityVec& EntityManager::getEntities(const EntityTags tag)
{
    return m_entityMap[tag];
}
