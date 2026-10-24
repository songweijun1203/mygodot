#pragma once

#include "DataObject.h"
#include "IGdSprite.h"

class IBubbleBase;
class ISprite : public XBaseObject{
protected:
    ISprite(const std::string& n):name(n){}
public:
    virtual ~ISprite() = default;
    virtual bool CreateGdSprite(const std::string& path,const XVec2& pos) = 0;
    virtual bool CreateGdBareSprite(const XVec2& pos) =0;
    virtual bool CreateGdBackdrop() =0;

    virtual sptr<IGdSprite> GetGdSprite()=0;

    std::string GetName() const{
        return name;
    }

    virtual sptr<IBubbleBase> GetTextBubble()  = 0;

    virtual sptr<IBubbleBase> GetQuoteBubble()  =0;
protected:
    std::string name;
};
