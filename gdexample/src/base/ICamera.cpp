#include "ICamera.h"

#include "base/IGdSprite.h"
#include "base/ISprite.h"

#include <cmath>

namespace {

/**
 * @brief 返回把 offset 收进 [-half, half] 区间所需的位移。
 *
 * offset 是人物相对相机中心的偏移。落在死区内时返回 0，也就是相机不动；
 * 超出死区时只返回超出的那一段，把人物正好拉回死区边缘。
 */
float PullBackToRange(float offset, float half)
{
    const float distance = std::fabs(offset);
    if (distance <= half)
    {
        return 0.0f;
    }

    return offset > 0.0f ? distance - half : half - distance;
}

} // namespace

void ICamera::SetFollowSprite(sptr<ISprite> s)
{
    followSprite = s;
}

sptr<ISprite> ICamera::GetFollowSprite() const
{
    return followSprite.lock();
}

void ICamera::SetFollowDeadzone(const XVec2 &deadzone)
{
    followDeadzone = deadzone;
}

void ICamera::SetFollowDamping(float damping)
{
    followDamping = damping;
}

void ICamera::Follow(float delta)
{
    auto sprite = GetFollowSprite();
    if (sprite == nullptr || sprite->GetGdSprite() == nullptr || delta <= 0.0f)
    {
        return;
    }

    const XVec2 camera_position = GetPosition();
    const XVec2 sprite_position = sprite->GetGdSprite()->GetPosition();

    // 可见世界尺寸 = 视口像素尺寸 / 缩放。死区按可见世界算，这样缩放变化时
    // 死区在屏幕上看起来始终是同一个大小。
    const XRect2 viewport = GetViewportRect();
    const XVec2 zoom = GetZoom();
    const float visible_width =
        zoom.x != 0.0f ? viewport.size.x / zoom.x : viewport.size.x;
    const float visible_height =
        zoom.y != 0.0f ? viewport.size.y / zoom.y : viewport.size.y;

    const float half_x = visible_width * 0.5f * followDeadzone.x;
    const float half_y = visible_height * 0.5f * followDeadzone.y;

    // 死区是中心对称的，所以这里不区分 SPX 的 Y 轴向上还是向下。
    const XVec2 wanted{
        camera_position.x + PullBackToRange(sprite_position.x - camera_position.x, half_x),
        camera_position.y + PullBackToRange(sprite_position.y - camera_position.y, half_y)};

    if (followDamping <= 0.0f)
    {
        SetPosition(wanted);
        return;
    }

    // 帧率无关的指数逼近：先算出这一帧该走完的比例，再动相机。
    const float ratio = 1.0f - std::exp(-followDamping * delta);
    SetPosition(XVec2{
        camera_position.x + (wanted.x - camera_position.x) * ratio,
        camera_position.y + (wanted.y - camera_position.y) * ratio});
}
