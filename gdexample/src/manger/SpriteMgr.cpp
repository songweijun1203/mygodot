#include "SpriteMgr.h"

#include "spx_api.h"
#include "base/xutils.h"

void SpriteMgr::OnStart()
{
}

void SpriteMgr::OnFixedUpdate(float delta)
{
}

void SpriteMgr::OnUpdate()
{
}

void SpriteMgr::OnDestroy()
{
}

void SpriteMgr::OnPause(bool isPaused)
{
}

Object_t SpriteMgr::CreateSprite(const std::string& path,const XVec2& pos)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_create_sprite == nullptr) {
        return 0;
    }

    spx_api::GdObj obj = 0;
    api.sprite_create_sprite(
            xutils::ToGdString(path), xutils::ToGdVec2(pos), &obj);
    return obj;
}

Object_t SpriteMgr::CreateBareSprite(const XVec2 &pos)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_create_bare_sprite == nullptr) {
        return 0;
    }

    spx_api::GdObj obj = 0;
    api.sprite_create_bare_sprite(xutils::ToGdVec2(pos), &obj);
    return obj;
}

Object_t SpriteMgr::CreateBackdrop(const std::string &path)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_create_backdrop == nullptr) {
        return 0;
    }

    spx_api::GdObj obj = 0;
    api.sprite_create_backdrop(xutils::ToGdString(path), &obj);
    return obj;
}

Object_t SpriteMgr::CloneSprite(Object_t id)
{
    if (id == 0) {
        return 0;
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_clone_sprite == nullptr) {
        return 0;
    }

    spx_api::GdObj clone_id = 0;
    api.sprite_clone_sprite(id, &clone_id);
    return clone_id;
}

bool SpriteMgr::DestroySprite(Object_t id)
{
    if (id == 0) {
        return false;
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_destroy_sprite == nullptr) {
        return false;
    }

    spx_api::GdBool destroyed = 0;
    api.sprite_destroy_sprite(id, &destroyed);
    return destroyed != 0;
}

bool SpriteMgr::IsSpriteAlive(Object_t id)
{
    if (id == 0) {
        return false;
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_is_sprite_alive == nullptr) {
        return false;
    }

    spx_api::GdBool alive = 0;
    api.sprite_is_sprite_alive(id, &alive);
    return alive != 0;
}

void SpriteMgr::SetDontDestroyOnLoad(Object_t id)
{
    if (id == 0) {
        return;
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_set_dont_destroy_on_load != nullptr) {
        api.sprite_set_dont_destroy_on_load(id);
    }
}

void SpriteMgr::SetTexture(Object_t id, const std::string &path)
{
    if (id == 0) {
        return;
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_set_texture != nullptr) {
        api.sprite_set_texture(id, xutils::ToGdString(path));
    }
}

void SpriteMgr::SetZIndex(Object_t id, int64_t index)
{
    if (id == 0) {
        return;
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_set_z_index != nullptr) {
        api.sprite_set_z_index(id, index);
    }
}

void SpriteMgr::SetPosition(Object_t id, const XVec2 &pos)
{
    if (id == 0) {
        return;
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_set_position != nullptr) {
        api.sprite_set_position(id, xutils::ToGdVec2(pos));
    }
}

XVec2 SpriteMgr::GetPosition(Object_t id) const
{
    if (id == 0) {
        return {};
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_get_position == nullptr) {
        return {};
    }

    spx_api::GdVec2 position{};
    api.sprite_get_position(id, &position);
    return xutils::ToXVec2(position);
}

void SpriteMgr::SetScale(Object_t id, const XVec2 &scale)
{
    if (id == 0) {
        return;
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_set_scale != nullptr) {
        api.sprite_set_scale(id, xutils::ToGdVec2(scale));
    }
}

void SpriteMgr::SetPhysicsMode(Object_t id, PhysicsMode mode)
{
    if (id == 0) {
        return;
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_set_physics_mode != nullptr) {
        api.sprite_set_physics_mode(id, (int64_t)mode);
    }
}

void SpriteMgr::SetVelocity(Object_t id, const XVec2 &velocity)
{
    if (id == 0) {
        return;
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_set_velocity != nullptr) {
        api.sprite_set_velocity(id, xutils::ToGdVec2(velocity));
    }
}

XVec2 SpriteMgr::GetVelocity(Object_t id) const
{
    auto &api = spx_api::GDExtensionInterface::get_instance();

    spx_api::GdVec2 v;
    api.sprite_get_velocity(id,&v);
    return XVec2{v.x,v.y};
}

void SpriteMgr::SetUseGravity(Object_t id, bool enabled)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    api.sprite_set_use_gravity(id,enabled);
}

void SpriteMgr::SetGravity(Object_t id, float gravity)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    api.sprite_set_gravity(id,gravity);
}

void SpriteMgr::SetGravityScale(Object_t id, float scale)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    api.sprite_set_gravity_scale(id,scale);
}

void SpriteMgr::SetMass(Object_t id, float mass)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    api.sprite_set_mass(id,mass);
}

void SpriteMgr::SetDrag(Object_t id, float drag)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    api.sprite_set_drag(id,drag);
}

void SpriteMgr::AddForce(Object_t id, const XVec2 &force)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    api.sprite_add_force(id,xutils::ToGdVec2(force));
}

void SpriteMgr::AddImpulse(Object_t id, const XVec2 &impulse)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    api.sprite_add_impulse(id,xutils::ToGdVec2(impulse));
}

void SpriteMgr::SetColliderCircle(Object_t id, const XVec2 &center, float radius)
{
    if (id == 0) {
        return;
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_set_collider_circle != nullptr) {
        api.sprite_set_collider_circle(id, xutils::ToGdVec2(center),radius);
    }
}

void SpriteMgr::SetColliderRect(Object_t id, const XVec2 &center, const XVec2 &size)
{
    if (id == 0) {
        return;
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_set_collider_rect != nullptr) {
        api.sprite_set_collider_rect(id, xutils::ToGdVec2(center),xutils::ToGdVec2(size));
    }
}

void SpriteMgr::SetColliderCapsule(Object_t id, const XVec2 &center, const XVec2 &size)
{
    if (id == 0) {
        return;
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_set_collider_capsule != nullptr) {
        api.sprite_set_collider_capsule(id, xutils::ToGdVec2(center),xutils::ToGdVec2(size));
    }
}

void SpriteMgr::SetCollisionMask(Object_t id, uint32_t mask)
{
    if (id == 0) {
        return;
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_set_collision_mask != nullptr) {
        api.sprite_set_collision_mask(id, mask);
    }
}

void SpriteMgr::SetCollisionLayer(Object_t id, uint32_t layer)
{
    if (id == 0) {
        return;
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_set_collision_layer != nullptr) {
        api.sprite_set_collision_layer(id, layer);
    }
}

void SpriteMgr::SetCollisionEnable(Object_t id,bool enable)
{
    if (id == 0) {
        return;
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_set_collision_enabled != nullptr) {
        api.sprite_set_collision_enabled(id, enable);
    }
}

void SpriteMgr::SetTriggerEnable(Object_t id,bool enable)
{
    if (id == 0) {
        return;
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_set_trigger_enabled != nullptr) {
        api.sprite_set_trigger_enabled(id, enable);
    }
}

void SpriteMgr::SetTriggerCircle(Object_t id,const XVec2 &center, float radius)
{
    if (id == 0) {
        return;
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_set_trigger_circle != nullptr) {
        api.sprite_set_trigger_circle(id, xutils::ToGdVec2(center),radius);
    }
}

void SpriteMgr::SetTriggerRect(Object_t id,const XVec2 &center, const XVec2 &size)
{
    if (id == 0) {
        return;
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_set_trigger_rect != nullptr) {
        api.sprite_set_trigger_rect(id, xutils::ToGdVec2(center),xutils::ToGdVec2(size));
    }
}

void SpriteMgr::SetTriggerCapsule(Object_t id,const XVec2 &center, const XVec2 &size)
{
    if (id == 0) {
        return;
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_set_trigger_capsule != nullptr) {
        api.sprite_set_trigger_capsule(id, xutils::ToGdVec2(center),xutils::ToGdVec2(size));
    }
}

void SpriteMgr::SetTriggerMask(Object_t id,uint32_t mask)
{
    if (id == 0) {
        return;
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_set_trigger_mask != nullptr) {
        api.sprite_set_trigger_mask(id, mask);
    }
}

void SpriteMgr::SetTriggerLayer(Object_t id,uint32_t layer)
{
    if (id == 0) {
        return;
    }

    auto &api = spx_api::GDExtensionInterface::get_instance();
    if (api.sprite_set_trigger_layer != nullptr) {
        api.sprite_set_trigger_layer(id, layer);
    }
}
