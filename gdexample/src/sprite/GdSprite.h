#pragma once

#include "base/IGdSprite.h"

class GdSprite : public IGdSprite
{
public:
    Object_t GetID() const override;
    void SetId(Object_t t) override;

    void SetTexture(const std::string& path) override;
    void SetZIndex(int64_t z) override;
    void SetPosition(const XVec2& pos) override;
    XVec2 GetPosition() const override;
    void SetScale(const XVec2& s) override;
    void SetPhysicsMode(PhysicsMode mode)override;
    
    void SetColliderCircle(const XVec2 &center, float radius)override;
    void SetColliderRect(const XVec2 &center, const XVec2 &size)override;
    void SetColliderCapsule(const XVec2 &center, const XVec2 &size)override;
    void SetCollisionMask(uint32_t mask)override;
    void SetCollisionLayer(uint32_t layer)override;

    // 速度
    void SetVelocity(const XVec2 &velocity)override;
    XVec2 GetVelocity() const override;

    // 重力
    void SetUseGravity(bool enabled) override;
    void SetGravity(float gravity) override;
    void SetGravityScale(float scale) override;

    // 质量和阻力
    void SetMass(float mass) override;
    void SetDrag(float drag) override;

    // 施力
    void AddForce(const XVec2 &force) override;
    void AddImpulse(const XVec2 &impulse) override;
protected:
    Object_t id;
};
