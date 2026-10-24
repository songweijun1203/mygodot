#pragma once

#include "DataObject.h"

class ISprite;
/**
 * @brief SPX 相机的高层抽象接口。
 *
 * 坐标和矩形类型使用 gdexample 自己的类型，具体实现负责把它们转换
 * 为 spx_api 中的 ABI 类型。坐标方向遵循 SPX 约定：X 轴向右，Y 轴向上。
 */
class ICamera : public XBaseObject {
public:
    virtual ~ICamera() = default;

    /**
     * @brief 获取相机在世界坐标中的位置。
     */
    virtual XVec2 GetPosition() const = 0;

    /**
     * @brief 设置相机在世界坐标中的位置。
     *
     * @param position 相机中心位置。
     */
    virtual void SetPosition(const XVec2 &position) = 0;

    /**
     * @brief 获取相机缩放系数。
     *
     * 缩放值为 {1, 1} 时表示原始比例；大于 1 表示放大画面，
     * 小于 1 表示缩小画面。
     */
    virtual XVec2 GetZoom() const = 0;

    /**
     * @brief 设置相机缩放系数。
     *
     * @param zoom 缩放系数，不是视口像素尺寸，不能包含 0。
     */
    virtual void SetZoom(const XVec2 &zoom) = 0;

    /**
     * @brief 获取视口矩形。
     *
     * 该矩形描述屏幕视口本身，通常位置为 {0, 0}，尺寸是视口尺寸。
     */
    virtual XRect2 GetViewportRect() const = 0;

    /**
     * @brief 获取当前相机可见的世界坐标矩形。
     */
    virtual XRect2 GetGlobalCameraRect() const = 0;

    /**
     * @brief 获取相机的舞台边界矩形。
     *
     * 该矩形来自相机的 left、top、right、bottom 限制，而不是当前可见区域。
     */
    virtual XRect2 GetStageLimitsRect() const = 0;

    /**
     * @brief 设置相机某一侧的世界坐标限制。
     *
     * @param side 边界编号：0 为左，1 为上，2 为右，3 为下。
     * @param limit 对应边界的世界坐标值。
     */
    virtual void SetCameraLimit(int64_t side, int64_t limit) = 0;

    /**
     * @brief 开关相机位置平滑。
     *
     * @param enabled 为 true 时启用位置平滑。
     */
    virtual void SetCameraSmoothing(bool enabled) = 0;

    void SetFollowSprite(sptr<ISprite>);

    sptr<ISprite> GetFollowSprite() const;

    private:
        wptr<ISprite> followSprite;
};
