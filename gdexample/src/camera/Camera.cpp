#include "Camera.h"

#include "spx_api.h"
#include "base/xutils.h"

namespace {

XVec2 ToXVec2(const spx_api::GdVec2 &value)
{
    return XVec2{value.x, value.y};
}

XRect2 ToXRect2(const spx_api::GdRect2 &value)
{
    return XRect2{
        XVec2{value.position.x, value.position.y},
        XVec2{value.size.x, value.size.y}};
}

} // namespace

XVec2 Camera::GetPosition() const
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.camera_get_camera_position == nullptr) {
        return {};
    }

    spx_api::GdVec2 position{};
    api.camera_get_camera_position(&position);
    return ToXVec2(position);
}

void Camera::SetPosition(const XVec2 &position)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.camera_set_camera_position == nullptr) {
        return;
    }

    api.camera_set_camera_position(xutils::ToGdVec2(position));
}

XVec2 Camera::GetZoom() const
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.camera_get_camera_zoom == nullptr) {
        return {};
    }

    spx_api::GdVec2 zoom{};
    api.camera_get_camera_zoom(&zoom);
    return ToXVec2(zoom);
}

void Camera::SetZoom(const XVec2 &zoom)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.camera_set_camera_zoom == nullptr) {
        return;
    }

    api.camera_set_camera_zoom(xutils::ToGdVec2(zoom));
}

XRect2 Camera::GetViewportRect() const
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.camera_get_viewport_rect == nullptr) {
        return {};
    }

    spx_api::GdRect2 rect{};
    api.camera_get_viewport_rect(&rect);
    return ToXRect2(rect);
}

XRect2 Camera::GetGlobalCameraRect() const
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.camera_get_global_camera_rect == nullptr) {
        return {};
    }

    spx_api::GdRect2 rect{};
    api.camera_get_global_camera_rect(&rect);
    return ToXRect2(rect);
}

XRect2 Camera::GetStageLimitsRect() const
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.camera_get_stage_limits_rect == nullptr) {
        return {};
    }

    spx_api::GdRect2 rect{};
    api.camera_get_stage_limits_rect(&rect);
    return ToXRect2(rect);
}

void Camera::SetCameraLimit(int64_t side, int64_t limit)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.camera_set_camera_limit == nullptr) {
        return;
    }

    api.camera_set_camera_limit(
        static_cast<spx_api::GdInt>(side),
        static_cast<spx_api::GdInt>(limit));
}

void Camera::SetCameraSmoothing(bool enabled)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.camera_set_camera_smoothing == nullptr) {
        return;
    }

    api.camera_set_camera_smoothing(enabled ? 1 : 0);
}
