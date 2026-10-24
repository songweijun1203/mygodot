#pragma once

#include "base/IManger.h"
#include<set>

/**
 * @brief SPX 精灵对象管理器。
 *
 * 通过 Object_t 保存 SpxSprite 的 ABI 对象 ID，避免在 gdexample 中直接
 * 持有 godot_modules 内部的 SpxSprite 指针。
 */
class SpriteMgr : public BaseManger {
public:
    virtual ~SpriteMgr() = default;
    virtual void OnStart() override;
    virtual void OnFixedUpdate(float delta) override;
    virtual void OnUpdate() override;
    virtual void OnDestroy() override;
    virtual void OnPause(bool isPaused) override;

    /**
     * @brief 从 SpxSprite 场景创建精灵。
     *
     * @param path SpxSprite 场景路径，例如 res://sprites/player.tscn。
     *             传入空字符串时创建运行时默认组件。
     * @param pos 精灵在场景中的初始位置。
     * @return 新精灵的 Object_t ID；创建失败时返回 0。
     */
    Object_t CreateSprite(const std::string &path, const XVec2 &pos);

    /**
     * @brief 创建带有默认运行时组件的空精灵。
     *
     * @param pos 精灵在场景中的初始位置。
     * @return 新精灵的 Object_t ID；创建失败时返回 0。
     */
    Object_t CreateBareSprite(const XVec2 &pos);

    /**
     * @brief 创建背景精灵。
     *
     * @param path SpxSprite 场景路径，不是纹理路径。
     * @return 新精灵的 Object_t ID；创建失败时返回 0。
     */
    Object_t CreateBackdrop(const std::string &path="");

    /**
     * @brief 克隆已有精灵及其当前状态。
     *
     * @param id 要克隆的精灵 Object_t ID。
     * @return 克隆精灵的 Object_t ID；源 ID 无效或克隆失败时返回 0。
     */
    Object_t CloneSprite(Object_t id);

    /**
     * @brief 请求销毁精灵。
     *
     * SPX 内部使用 queue_free()，因此该函数返回成功只表示销毁请求已提交。
     *
     * @param id 要销毁的精灵 Object_t ID。
     * @return 销毁请求是否成功提交。
     */
    bool DestroySprite(Object_t id);

    /**
     * @brief 检查精灵对象是否仍然有效。
     *
     */
    bool IsSpriteAlive(Object_t id);

    /**
     * @brief 将精灵移动到 SPX 的持久节点下。
     */
    void SetDontDestroyOnLoad(Object_t id);

    void SetTexture(Object_t id, const std::string &path);

    /**
     * @brief 设置精灵的Z,数值大的在上面
     */
    void SetZIndex(Object_t id,int64_t index);

    void SetPosition(Object_t id,const XVec2& pos);

    XVec2 GetPosition(Object_t id) const;

    void SetScale(Object_t id,const XVec2& pos);

    //关于物理相关的忌口
    void SetPhysicsMode(Object_t id,PhysicsMode mode);
    
    void SetColliderCircle(Object_t id,const XVec2 &center, float radius);
    void SetColliderRect(Object_t id,const XVec2 &center, const XVec2 &size);
    void SetColliderCapsule(Object_t id,const XVec2 &center, const XVec2 &size);
    void SetCollisionMask(Object_t id,uint32_t mask);
    void SetCollisionLayer(Object_t id,uint32_t layer);

    // 速度
    void SetVelocity(Object_t id,const XVec2 &velocity);
    XVec2 GetVelocity(Object_t id) const;

    // 重力
    void SetUseGravity(Object_t id,bool enabled);
    void SetGravity(Object_t id,float gravity);
    void SetGravityScale(Object_t id,float scale);

    // 质量和阻力
    void SetMass(Object_t id,float mass);
    void SetDrag(Object_t id,float drag);

    // 施力
    void AddForce(Object_t id,const XVec2 &force);
    void AddImpulse(Object_t id,const XVec2 &impulse);
};
