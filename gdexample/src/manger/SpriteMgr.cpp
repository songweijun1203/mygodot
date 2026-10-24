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
