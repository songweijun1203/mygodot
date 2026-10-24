#pragma once

#include "base/DataObject.h"
#include <vector>

class IGdSprite;
class IGdPen : public XBaseObject
{
public:
    virtual Object_t GetId() const =0;
    virtual void SetId(Object_t id) = 0;
    virtual void CreatePen() = 0;
    
    //也许叫beginPaint endPaint更好
    virtual void PenDown() =0;  
    virtual void PenMove(const XVec2& pos) =0;
    virtual void PenUp()=0;
    virtual void SetPenColor(float r,float g,float b,float a) =0;
    virtual void SetPenSize(float size) =0;
    virtual void DrawPolygons(const std::vector<XVec2>&points) =0;

    /*
    * @brief 清除所有笔迹，不仅仅是当前画笔
    */
    virtual void EraseAll() =0;

    virtual void PenStamp() =0;
    virtual void PenStampSprite(sptr<IGdSprite> sprite) =0;
};