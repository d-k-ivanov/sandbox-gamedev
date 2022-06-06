#include "Entity.h"

Entity::Entity(const size_t id, const EntityTags tag) : m_id(id), m_tag(tag)
{
}


bool Entity::isActive() const
{
    return m_active;
}

EntityTags Entity::tag() const
{
    return m_tag;
}

size_t Entity::id() const
{
    return m_id;
}

void Entity::destroy()
{
    m_active = false;
}
