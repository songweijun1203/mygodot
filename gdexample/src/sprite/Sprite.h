#pragma once

#include "base/ISprite.h"
#include "GdSprite.h"

class Sprite : public ISprite
{
public:
    Sprite(const std::string& n);
public:
    bool CreateGdSprite(const std::string& path,const XVec2& pos) override;

    bool CreateGdBareSprite(const XVec2& pos) override;

    bool CreateGdBackdrop() override;
};