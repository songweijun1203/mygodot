#include "PenMgr.h"
#include "spx_api.h"

Object_t PenMgr::CreatePen()
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    spx_api::GdObj ret;
    api.pen_create_pen(&ret);
    return ret;
}

void PenMgr::PenDown(Object_t id)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    api.pen_pen_down(id,false);
}

void PenMgr::PenMove(Object_t id, const XVec2 &pos)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    api.pen_move_pen_to(id,xutils::ToGdVec2(pos));
}

void PenMgr::PenUp(Object_t id)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    api.pen_pen_up(id);
}

void PenMgr::DestoryPen(Object_t id)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    api.pen_destroy_pen(id);
}

void PenMgr::EraseAll()
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    api.pen_destroy_all_pens();
}

void PenMgr::SetPenSize(Object_t id, float size)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    api.pen_set_pen_size_to(id,size);
}

void PenMgr::SetPenColor(Object_t id,float r, float g, float b, float a)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    api.pen_set_pen_color_to(id,spx_api::GdColor{r,g,b,a});
}

void PenMgr::PenStamp(Object_t id)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    api.pen_pen_stamp(id);
}

void PenMgr::PenStampSprite(Object_t sprite_id)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    api.pen_pen_stamp_sprite(sprite_id);
}
