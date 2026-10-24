#pragma once

#include "DataObject.h"
#include "IGdSprite.h"

class ISprite : public XBaseObject{
protected:
    ISprite(const std::string& n):name(n){}
public:
    virtual ~ISprite() = default;
    virtual bool CreateGdSprite(const std::string& path,const XVec2& pos) = 0;
    virtual bool CreateGdBareSprite(const XVec2& pos) =0;
    virtual bool CreateGdBackdrop() =0;

    sptr<IGdSprite> GetGdSprite(){
        return gdsprite;
    }

    std::string GetName() const{
        return name;
    }
protected:
    sptr<IGdSprite> gdsprite;
    std::string name;
};
