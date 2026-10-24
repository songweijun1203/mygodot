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
protected:
    Object_t id;
};