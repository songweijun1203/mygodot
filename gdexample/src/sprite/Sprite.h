#pragma once

#include "base/ISprite.h"
#include "GdSprite.h"

class Sprite : public ISprite
{
public:
    Sprite(const std::string& n);
    ~Sprite();
public:
    //bool CreateGdSprite(const std::string& path,const XVec2& pos) override;

    sptr<IGdSprite> GetOrCreateGdSprite(const SpriteType& type) override;
    sptr<IGdSprite> GetGdSprite() override;

    sptr<IGdAudio> GetOrCreateAudio() override;

    //bool CreateGdBareSprite(const XVec2& pos) override;

    //bool CreateGdBackdrop() override;

    sptr<IBubbleBase> GetOrCreateTextBubble() override;

    sptr<IBubbleBase> GetOrCreateQuoteBubble() override;

protected:
    class Internal;
    std::unique_ptr<Internal> mData;
};