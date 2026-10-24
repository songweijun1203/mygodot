#pragma once

#include "base/IManger.h"
#include<set>

class PenMgr : public BaseManger {
 public:
    Object_t CreatePen();

    void PenDown(Object_t id);  
    void PenMove(Object_t id,const XVec2& pos);
    void PenUp(Object_t id);
    void DestoryPen(Object_t id);
    void EraseAll();
    void SetPenSize(Object_t id,float size);
    void SetPenColor(Object_t id,float r,float g,float b,float a);

    void PenStamp(Object_t id);
    void PenStampSprite(Object_t sprite_id);
};