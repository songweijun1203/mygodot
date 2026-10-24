#pragma once

#include "DataObject.h"

class IGdSprite : public XBaseObject{
public:
    virtual ~IGdSprite() = default;
    virtual Object_t GetID() const =0;
    virtual void SetId(Object_t) = 0;
    virtual void SetTexture(const std::string& path)=0;
    virtual void SetZIndex(int64_t z) =0;
    virtual void SetPosition(const XVec2& pos) =0;
};
