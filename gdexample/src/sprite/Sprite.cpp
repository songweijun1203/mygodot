#include "Sprite.h"
#include "DataManger.h"
#include "manger/SpriteMgr.h"
#include "ui/QuoteBubble.h"

class Sprite::Internal{
public:
    sptr<IGdSprite> gdsprite;
    sptr<QuotoBubble> quoteBubble;
};
Sprite::Sprite(const std::string &n):ISprite(n),mData(new Internal)
{
}

Sprite::~Sprite()
{
}

bool Sprite::CreateGdSprite(const std::string &path, const XVec2 &pos)
{   
    auto spriteMgr = DataManger::getInstance().getSpriteMgr()->asDerived<SpriteMgr>();
    if(!spriteMgr)
        return false;
    auto id = spriteMgr->CreateSprite(path,pos);
    mData->gdsprite = makeShareObject<GdSprite>();
    mData->gdsprite->SetId(id);
    return id != 0;
}

bool Sprite::CreateGdBareSprite(const XVec2 &pos)
{
     auto spriteMgr = DataManger::getInstance().getSpriteMgr()->asDerived<SpriteMgr>();
    if(!spriteMgr)
        return false;
    auto id = spriteMgr->CreateBareSprite(pos);
    mData->gdsprite = makeShareObject<GdSprite>();
    mData->gdsprite->SetId(id);
    return id != 0;
}

bool Sprite::CreateGdBackdrop()
{
     auto spriteMgr = DataManger::getInstance().getSpriteMgr()->asDerived<SpriteMgr>();
    if(!spriteMgr)
        return false;

    auto id = spriteMgr->CreateBackdrop();
    mData->gdsprite = makeShareObject<GdSprite>();
    mData->gdsprite->SetId(id);
    return id != 0;
}

sptr<IGdSprite> Sprite::GetGdSprite()
{
    return mData->gdsprite;
}

sptr<IBubbleBase> Sprite::GetTextBubble()
{
    
}

sptr<IBubbleBase> Sprite::GetQuoteBubble()
{
    if(!mData->quoteBubble){
        mData->quoteBubble = makeShareObject<QuotoBubble>();
        mData->quoteBubble->SetSprite(asDerived<Sprite>());
        mData->quoteBubble->CreateUiCompnonent();
    }
    return mData->quoteBubble;
}
