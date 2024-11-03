#pragma once

#include <iostream>
#include <algorithm>
#include <utility>

#include "Entity.hpp"

// macro for creating and empty vector of Entity (shared pointer).
using EntityVec = std::vector<std::shared_ptr<Entity>>;

class EntityManager
{
private:
    EntityVec m_entities;
    EntityVec m_entitiesToAdd;
    std::map<std::string, EntityVec> m_entityMap;
    size_t m_totalEntities = 0;

    void removeDeadEntities(EntityVec& vec)
    {
        // the vector erase method can be used in conjunction with std::remove_if
        vec.erase(
            std::remove_if(
                vec.begin(), // iterator start 
                vec.end(), // iterator end
                [](auto entity) {return !entity->isActive();} // lambda function 
                ),
            vec.end()
        );
        
    }
public:
    EntityManager() = default;
    
    void update()
    {
        m_entities.insert(
            m_entities.end(), // where in the vector 
            std::make_move_iterator(m_entitiesToAdd.begin()), // iterator start 
            std::make_move_iterator(m_entitiesToAdd.end())
            );  //iterator end

        m_entitiesToAdd.clear();
        
        // // remove dead entities from the vector of all entities.
        removeDeadEntities(m_entities);

        // // remove dead entities from each vector int the entity map
        // // C++20 way of iterating through [key, value] pairs in a map
        for  (auto& [tag, entityVec] : m_entityMap)
        {
            removeDeadEntities(entityVec);
        }
    }

    std::shared_ptr<Entity> addEntity(const std::string& tag)
    {
        // creat the entity shared pointer
        auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

        // add it to the entity map
        if (m_entityMap.find(tag) == m_entityMap.end())
        {
            m_entityMap[tag] = EntityVec();
        }
        m_entityMap[tag].push_back(entity);
        m_entitiesToAdd.push_back(entity);
        return entity;
    }

    const EntityVec& getEntities()
    {   
        return m_entities;
    }

    const EntityVec& getEntities(const std::string& tag)
    {
        if (m_entityMap.find(tag) == m_entityMap.end())
        {
            m_entityMap[tag] = EntityVec();
        }
        return m_entityMap[tag];
    }

    const std::map<std::string, EntityVec>& getEntityMap()
    {
        return m_entityMap;
    }
};