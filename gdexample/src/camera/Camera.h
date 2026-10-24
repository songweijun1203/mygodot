#pragma once

#include "base/ICamera.h"

/**
 * @brief 基于 SPX ABI 相机接口的具体实现。
 */
class Camera : public ICamera {
public:
    ~Camera() override = default;

    XVec2 GetPosition() const override;
    void SetPosition(const XVec2 &position) override;

    XVec2 GetZoom() const override;
    void SetZoom(const XVec2 &zoom) override;

    XRect2 GetViewportRect() const override;
    XRect2 GetGlobalCameraRect() const override;
    XRect2 GetStageLimitsRect() const override;

    void SetCameraLimit(int64_t side, int64_t limit) override;
    void SetCameraSmoothing(bool enabled) override;

};
