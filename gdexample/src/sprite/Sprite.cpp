#include "Sprite.h"
#include "DataManger.h"
#include "manger/SpriteMgr.h"

Sprite::Sprite(const std::string &n):ISprite(n)
{
}

bool Sprite::CreateGdSprite(const std::string &path, const XVec2 &pos)
{   
    auto spriteMgr = DataManger::getInstance().getSpriteMgr()->asDerived<SpriteMgr>();
    if(!spriteMgr)
        return false;
    auto id = spriteMgr->CreateSprite(path,pos);
    gdsprite = makeShareObject<GdSprite>();
    gdsprite->SetId(id);
    return id != 0;
}

bool Sprite::CreateGdBareSprite(const XVec2 &pos)
{
     auto spriteMgr = DataManger::getInstance().getSpriteMgr()->asDerived<SpriteMgr>();
    if(!spriteMgr)
        return false;
    auto id = spriteMgr->CreateBareSprite(pos);
    gdsprite = makeShareObject<GdSprite>();
    gdsprite->SetId(id);
    return id != 0;
}

bool Sprite::CreateGdBackdrop()
{
     auto spriteMgr = DataManger::getInstance().getSpriteMgr()->asDerived<SpriteMgr>();
    if(!spriteMgr)
        return false;

    auto id = spriteMgr->CreateBackdrop();
    gdsprite = makeShareObject<GdSprite>();
    gdsprite->SetId(id);
    return id != 0;
}
