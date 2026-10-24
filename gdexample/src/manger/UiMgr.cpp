#include "UiMgr.h"
#include "spx_api.h"
#include "base/xutils.h"

Object_t UiMgr::CreateUiNode(const std::string &path)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.ui_create_node == nullptr)
    {
        return 0;
    }

    spx_api::GdObj obj = 0;
    api.ui_create_node(
        xutils::ToGdString(path), &obj);
    return obj;
}

Object_t UiMgr::BindNode(Object_t parentId, const std::string &relative_path)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    spx_api::GdObj obj = 0;
    api.ui_bind_node(parentId, xutils::ToGdString(relative_path), &obj);
    return obj;
}

void UiMgr::SetText(Object_t object_id, const std::string &text)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    spx_api::GdObj obj = 0;
    api.ui_set_text(object_id, xutils::ToGdString(text));
}

float UiMgr::SetTextMaxWidth(Object_t object_id, const std::string &text, float width)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    spx_api::GdFloat w;
    api.ui_set_text_max_width(object_id, xutils::ToGdString(text), width, &w);
    return w;
}

void UiMgr::SetVisible(Object_t object_id, bool visible)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    api.ui_set_visible(object_id, visible);
}

void UiMgr::SetFontSize(Object_t object_id, int64_t size)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    api.ui_set_font_size(object_id, size);
}

void UiMgr::SetScale(Object_t object_id, const XVec2 &scale)
{
    if (object_id == 0)
    {
        return;
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.ui_set_scale != nullptr)
    {
        api.ui_set_scale(object_id, xutils::ToGdVec2(scale));
    }
}

XVec2 UiMgr::GetScale(Object_t object_id)
{
    if (object_id == 0)
    {
        return {};
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.ui_get_scale != nullptr)
    {
        spx_api::GdVec2 ret_value;
        api.ui_get_scale(object_id, &ret_value);
        return XVec2{ret_value.x, ret_value.y};
    }
    return {};
}

XVec2 UiMgr::GetCanvasScale(Object_t object_id)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    spx_api::GdVec2 ret_value;
    api.ui_get_canvas_scale(object_id, &ret_value);
    return XVec2{ret_value.x, ret_value.y};
}

void UiMgr::SetPosition(Object_t object_id, const XVec2 &position)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    api.ui_set_position(object_id,xutils::ToGdVec2(position));
}

XVec2 UiMgr::GetPosition(Object_t object_id) const
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    spx_api::GdVec2 ret_value;
    api.ui_get_position(object_id, &ret_value);
    return XVec2{ret_value.x, ret_value.y};
}

bool UiMgr::DeleteUiNode(Object_t object_id) const
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    spx_api::GdBool ret;
    api.ui_destroy_node(object_id,&ret);
    return ret;
}
