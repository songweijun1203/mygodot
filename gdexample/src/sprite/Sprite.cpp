#include "Sprite.h"
#include "DataManger.h"
#include "manger/SpriteMgr.h"
#include "other/GdAudio.h"
#include "ui/QuoteBubble.h"

class Sprite::Internal{
public:
    sptr<IGdSprite> gdsprite;
    sptr<IGdAudio> audio;
    sptr<QuotoBubble> quoteBubble;
};
Sprite::Sprite(const std::string &n):ISprite(n),mData(new Internal)
{
}

Sprite::~Sprite()
{
}

#if 0
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
#endif

sptr<IGdSprite> Sprite::GetOrCreateGdSprite(const SpriteType& type)
{   
    if(!mData->gdsprite){
        mData->gdsprite = makeShareObject<GdSprite>();
        mData->gdsprite->CreateSprite(type);
    }
    return mData->gdsprite;
}

sptr<IGdSprite> Sprite::GetGdSprite()
{
    return mData->gdsprite;
}

sptr<IGdAudio> Sprite::GetOrCreateAudio()
{
    if(!mData->audio){
        mData->audio = makeShareObject<GdAudio>();
        mData->audio->CreateAudio();   // 向引擎申请音频组，之后播放才有效
    }
    return mData->audio;
}

sptr<IBubbleBase> Sprite::GetOrCreateTextBubble()
{
    // TODO: 文本气泡尚未接入；返回 nullptr 而非掉出函数尾（未定义行为）
    return nullptr;
}

sptr<IBubbleBase> Sprite::GetOrCreateQuoteBubble()
{
    if(!mData->quoteBubble){
        mData->quoteBubble = makeShareObject<QuotoBubble>();
        mData->quoteBubble->SetSprite(asDerived<Sprite>());
        mData->quoteBubble->CreateUiCompnonent();
    }
    return mData->quoteBubble;
}
