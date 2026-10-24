#pragma once

#include "base/ISprite.h"
#include "GdSprite.h"

class Sprite : public ISprite
{
public:
    Sprite(const std::string& n);
    ~Sprite();
public:
    bool CreateGdSprite(const std::string& path,const XVec2& pos) override;

    sptr<IGdSprite> GetGdSprite() override;

    bool CreateGdBareSprite(const XVec2& pos) override;

    bool CreateGdBackdrop() override;

    sptr<IBubbleBase> GetTextBubble() override;

    sptr<IBubbleBase> GetQuoteBubble() override;

protected:
    class Internal;
    std::unique_ptr<Internal> mData;
};