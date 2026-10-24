#include "GdPen.h"
#include "manger/PenMgr.h"
#include "DataManger.h"
#include "base/ISprite.h"
#include "gdbase/IGdSprite.h"

class GdPen::Internal{
public:
    Object_t id;
};
GdPen::GdPen():mData(new Internal)
{
}

GdPen::~GdPen()
{
}

Object_t GdPen::GetId() const
{
    return mData->id;
}

void GdPen::SetId(Object_t id)
{
    mData->id = id;
}

void GdPen::CreatePen()
{
    auto pen_mgr = DataManger::getInstance().getPenMgr()->asDerived<PenMgr>();
    SetId(pen_mgr->CreatePen());
}

void GdPen::PenDown()
{
    auto pen_mgr = DataManger::getInstance().getPenMgr()->asDerived<PenMgr>();
    pen_mgr->PenDown(GetId());
}

void GdPen::PenMove(const XVec2 &pos)
{
    auto pen_mgr = DataManger::getInstance().getPenMgr()->asDerived<PenMgr>();
    pen_mgr->PenMove(GetId(),pos);
}

void GdPen::PenUp()
{
    auto pen_mgr = DataManger::getInstance().getPenMgr()->asDerived<PenMgr>();
    pen_mgr->PenUp(GetId());
}

void GdPen::SetPenColor(float r, float g, float b, float a)
{
    auto pen_mgr = DataManger::getInstance().getPenMgr()->asDerived<PenMgr>();
    pen_mgr->SetPenColor(GetId(),r,g,b,a);
}

void GdPen::SetPenSize(float size)
{
    auto pen_mgr = DataManger::getInstance().getPenMgr()->asDerived<PenMgr>();
    pen_mgr->SetPenSize(GetId(),size);
}

void GdPen::DrawPolygons(const std::vector<XVec2> &points)
{
    if (points.size() < 2) { return; }
    PenMove(points[0]);        // 抬笔态 = 只定位
    PenDown();
    for (size_t i = 1; i < points.size(); ++i) { PenMove(points[i]); }
    PenUp();
}

void GdPen::EraseAll()
{
    auto pen_mgr = DataManger::getInstance().getPenMgr()->asDerived<PenMgr>();
    pen_mgr->EraseAll();
}

void GdPen::PenStamp()
{
    auto pen_mgr = DataManger::getInstance().getPenMgr()->asDerived<PenMgr>();
    pen_mgr->PenStamp(GetId());
}

void GdPen::PenStampSprite(sptr<IGdSprite> sprite)
{
    auto pen_mgr = DataManger::getInstance().getPenMgr()->asDerived<PenMgr>();
    pen_mgr->PenStampSprite(sprite->GetID());
}
