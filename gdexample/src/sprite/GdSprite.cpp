#include "GdSprite.h"

#include "DataManger.h"
#include "manger/SpriteMgr.h"

static sptr<SpriteMgr> GetSpriteMgr()
{
    auto sprite_mgr_base = DataManger::getInstance().getSpriteMgr();
    if (!sprite_mgr_base)
    {
        return nullptr;
    }

    auto sprite_mgr = sprite_mgr_base->asDerived<SpriteMgr>();

    return sprite_mgr;
}
Object_t GdSprite::GetID() const
{
    return id;
}

void GdSprite::SetId(Object_t t)
{
    id = t;
}

void GdSprite::CreateSprite(const SpriteType& type)
{
    auto sprite_mgr_base = DataManger::getInstance().getSpriteMgr();
    auto sprite_mgr = sprite_mgr_base->asDerived<SpriteMgr>();
    switch (type)
    {
    case SpriteType::sprite:
        SetId(sprite_mgr->CreateSprite("",XVec2{0,0}));
        break;
    case SpriteType::backdrop:
        SetId(sprite_mgr->CreateBackdrop(""));
        break;
    default:
        break;
    }
    //sprite_mgr->CreateSprite("",XVec2{0,0});
}

void GdSprite::SetTexture(const std::string &path)
{
    if (id == 0 || path.empty())
    {
        return;
    }

    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->SetTexture(id, path);
    }
}

void GdSprite::SetZIndex(int64_t z)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->SetZIndex(id, z);
    }
}

void GdSprite::SetPosition(const XVec2 &pos)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->SetPosition(id, pos);
    }
}

XVec2 GdSprite::GetPosition() const
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        return sprite_mgr->GetPosition(id);
    }

    return {};
}

void GdSprite::SetScale(const XVec2 &s)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->SetScale(id, s);
    }
}

void GdSprite::SetPhysicsMode(PhysicsMode mode)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->SetPhysicsMode(id, mode);
    }
}

void GdSprite::SetVelocity(const XVec2 &velocity)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->SetVelocity(id, velocity);
    }
}

XVec2 GdSprite::GetVelocity() const
{
    XVec2 v{};
    if (auto sprite_mgr = GetSpriteMgr())
    {
        v = sprite_mgr->GetVelocity(id);
    }
    return v;
}

void GdSprite::SetUseGravity(bool enabled)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->SetUseGravity(id, enabled);
    }
}

void GdSprite::SetGravity(float gravity)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->SetGravity(id, gravity);
    }
}

void GdSprite::SetGravityScale(float scale)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->SetGravityScale(id, scale);
    }
}

void GdSprite::SetMass(float mass)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->SetMass(id, mass);
    }
}

void GdSprite::SetDrag(float drag)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->SetDrag(id, drag);
    }
}

void GdSprite::AddForce(const XVec2 &force)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->AddForce(id, force);
    }
}

void GdSprite::AddImpulse(const XVec2 &impulse)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->AddImpulse(id, impulse);
    }
}

void GdSprite::SetColliderCircle(const XVec2 &center, float radius)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->SetColliderCircle(id, center,radius);
    }
}

void GdSprite::SetColliderRect(const XVec2 &center, const XVec2 &size)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->SetColliderRect(id, center,size);
    }
}

void GdSprite::SetColliderCapsule(const XVec2 &center, const XVec2 &size)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->SetColliderCapsule(id, center,size);
    }
}

void GdSprite::SetCollisionMask(uint32_t mask)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->SetCollisionMask(id, mask);
    }
}

void GdSprite::SetCollisionLayer(uint32_t layer)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->SetCollisionLayer(id, layer);
    }
}

void GdSprite::SetCollisionEnable(bool enable)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->SetCollisionEnable(id, enable);
    }
}

void GdSprite::SetTriggerEnable(bool enable)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->SetTriggerEnable(id, enable);
    }
}

void GdSprite::SetTriggerCircle(const XVec2 &center, float radius)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->SetTriggerCircle(id, center,radius);
    }
}

void GdSprite::SetTriggerRect(const XVec2 &center, const XVec2 &size)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->SetTriggerRect(id, center,size);
    }
}

void GdSprite::SetTriggerCapsule(const XVec2 &center, const XVec2 &size)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->SetTriggerCapsule(id, center,size);
    }
}

void GdSprite::SetTriggerMask(uint32_t mask)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->SetTriggerMask(id, mask);
    }
}

void GdSprite::SetTriggerLayer(uint32_t layer)
{
    if (auto sprite_mgr = GetSpriteMgr())
    {
        sprite_mgr->SetTriggerLayer(id, layer);
    }
}
