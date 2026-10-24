#pragma once

#include "DataObject.h"

class IGdUiNode:public XBaseObject{
public:
    virtual Object_t GetId() const =0;
    virtual void SetId(Object_t id) = 0;

    virtual void SetVisible(bool visible) =0;
    virtual void SetText(const std::string& text) = 0;
    virtual void SetScale(const XVec2& scale) = 0;
    virtual XVec2 GetScale()const=0;
    virtual XVec2 GetCanvasScale()const=0;
    virtual float SetTextMaxWidth(const std::string& text,float width) =0;
    virtual void SetPosition(const XVec2& scale) =0;
    virtual XVec2 GetPosition() const =0;

    virtual bool DeleteLater() =0;
};