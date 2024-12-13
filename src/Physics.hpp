#pragma once

#include "Entity.hpp"

namespace Physics
{
    // see collision class...
    Vec2f GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
    {
        if (a->has<CBoundingBox>() && b->has<CBoundingBox>())
        {
            auto& a_bbox = a->get<CBoundingBox>();
            auto& b_bbox = b->get<CBoundingBox>();
            auto& a_transform = a->get<CTransform>();
            auto& b_transform = b->get<CTransform>();

            Vec2f delta = b_transform.pos - a_transform.pos;
            Vec2f bb_threshold = a_bbox.halfSize + b_bbox.halfSize;
            float ox = abs(delta.x) - bb_threshold.x;
            float oy = abs(delta.y) - bb_threshold.y;
            return Vec2f(ox, oy);
        }
        return Vec2f(0.0f, 0.0f);
    }

    Vec2f GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
    {

        if (a->has<CBoundingBox>() && b->has<CBoundingBox>())
        {
            auto& a_bbox = a->get<CBoundingBox>();
            auto& b_bbox = b->get<CBoundingBox>();
            auto& a_transform = a->get<CTransform>();
            auto& b_transform = b->get<CTransform>();

            Vec2f delta = b_transform.prevPos - a_transform.prevPos;
            Vec2f bb_threshold = a_bbox.halfSize + b_bbox.halfSize;
            float ox = abs(delta.x) -  bb_threshold.x;
            float oy = abs(delta.y) - bb_threshold.y;
            return Vec2f(ox, oy);
        }
        return Vec2f(0.0f, 0.0f);
    }
}