#pragma once

#include "DataObject.h"

class IUiComponent;
class ISprite;
class IBubbleBase:public XBaseObject{
public:
    virtual sptr<IUiComponent> GetUiCompnonent() const= 0;
    virtual void CreateUiCompnonent()=0;
    virtual sptr<ISprite> GetSprite() const =0;
    virtual void SetSprite(sptr<ISprite> sprite)=0;
};