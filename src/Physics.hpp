#pragma once

#include "Entity.hpp"

namespace Physics
{
    // see collision class...
    Vec2f GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
    {
        // TODO return the overlap rectangle size of the bounding intersection.
        return Vec2f(0.0, 0.0);
    }

    Vec2f GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
    {
        // TODO return the previous overlap using the entity previous position.
        return Vec2f(0.0, 0.0);
    }
}