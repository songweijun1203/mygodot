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
