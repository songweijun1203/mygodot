#pragma once

#include "DataObject.h"

class IGdSprite : public XBaseObject
{
public:
    virtual ~IGdSprite() = default;
    virtual Object_t GetID() const = 0;
    virtual void SetId(Object_t) = 0;
    virtual void SetTexture(const std::string &path) = 0;
    virtual void SetZIndex(int64_t z) = 0;
    virtual void SetPosition(const XVec2 &pos) = 0;
    virtual XVec2 GetPosition() const = 0;
    virtual void SetScale(const XVec2 &pos) = 0;

    // 物理模式
    virtual void SetPhysicsMode(PhysicsMode mode) = 0;

    // 碰撞
    virtual void SetColliderCircle(const XVec2 &center, float radius) = 0;
    virtual void SetColliderRect(const XVec2 &center, const XVec2 &size) = 0;
    virtual void SetColliderCapsule(const XVec2 &center, const XVec2 &size) = 0;
    virtual void SetCollisionMask(uint32_t mask) = 0;
    virtual void SetCollisionLayer(uint32_t layer) = 0;

    // 速度
    virtual void SetVelocity(const XVec2 &velocity) = 0;
    virtual XVec2 GetVelocity() const = 0;

    // 重力
    virtual void SetUseGravity(bool enabled) = 0;
    virtual void SetGravity(float gravity) = 0;
    virtual void SetGravityScale(float scale) = 0;

    // 质量和阻力
    virtual void SetMass(float mass) = 0;
    virtual void SetDrag(float drag) = 0;

    // 施力
    virtual void AddForce(const XVec2 &force) = 0;
    virtual void AddImpulse(const XVec2 &impulse) = 0;
};
