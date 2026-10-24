#include "Game.h"

#include "gdexample.h"
#include "base/ISprite.h"
#include "DataManger.h"
#include "manger/SpriteMgr.h"
#include "sprite/Sprite.h"
#include "camera/Camera.h"
#include "spx_api.h"

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/core/memory.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/viewport.hpp>

#include <cmath>
#include <vector>

#include <iostream>

class Game::Internal
{
public:
    Internal()
    {
        camera = makeShareObject<Camera>();
    }

public:
    godot::Node *root;
    std::vector<sptr<ISprite>> sprites;
    sptr<Camera> camera;

    bool move_left = false;
    bool move_right = false;
    bool move_up = false;
    bool move_down = false;
};

Game::Game() : mData(new Game::Internal())
{
}

Game::~Game()
{
}

void Game::OnEngineStart(godot::Node *SpxEngineNode)
{
    mData->root = SpxEngineNode;

    // 使用碰撞形状判断触发器，不根据纹理透明像素判断重叠。
    auto &spx = spx_api::GDExtensionInterface::get_instance();
    if (spx.physics_set_collision_system_type != nullptr)
    {
        spx.physics_set_collision_system_type(false);
    }

    {
        auto *viewport = SpxEngineNode->get_viewport();
        if (viewport != nullptr)
        {
            const godot::Vector2 size =
                viewport->get_visible_rect().get_size();
        }

        auto *settings = godot::ProjectSettings::get_singleton();

        const int64_t design_width = static_cast<int64_t>(
            settings->get_setting(
                "display/window/size/viewport_width",
                640));

        const int64_t design_height = static_cast<int64_t>(
            settings->get_setting(
                "display/window/size/viewport_height",
                360));
    }
    // 设置游戏精灵
    {
        auto player = makeShareObject<Sprite>("player");
        player->CreateGdSprite("", XVec2{0, 0});

        player->GetGdSprite()->SetTexture("res://assets/player.png");

        player->GetGdSprite()->SetZIndex(0);

        player->GetGdSprite()->SetPhysicsMode(PhysicsMode::Dynamic);
        // 重力
        player->GetGdSprite()->SetUseGravity(true);
        player->GetGdSprite()->SetGravity(980.0f);
        player->GetGdSprite()->SetGravityScale(0.0f);
        // 物理参数
        player->GetGdSprite()->SetMass(1.0f);
        player->GetGdSprite()->SetDrag(2.0f);

        // 玩家实体碰撞形状 碰撞过滤
        player->GetGdSprite()->SetCollisionEnable(true);
        player->GetGdSprite()->SetColliderCircle(XVec2{0.0f, 0.0f}, 32.0f);
        player->GetGdSprite()->SetCollisionLayer(1);
        player->GetGdSprite()->SetCollisionMask(2);

        player->GetGdSprite()->SetTriggerCircle(XVec2{0.0f, 0.0f}, 60.0f);
        player->GetGdSprite()->SetTriggerLayer(1);
        player->GetGdSprite()->SetTriggerMask(2);

        mData->sprites.push_back(player);
    }
    {
        auto rock = makeShareObject<Sprite>("rock");
        rock->CreateGdSprite("", XVec2{0, 0});

        rock->GetGdSprite()->SetTexture("res://assets/rock_transparent.png");

        rock->GetGdSprite()->SetZIndex(0);

        rock->GetGdSprite()->SetPosition(XVec2{200, 200});

        rock->GetGdSprite()->SetScale(XVec2{0.1f, 0.1f});

        rock->GetGdSprite()->SetPhysicsMode(PhysicsMode::Static);
        // 根节点缩放为 0.1，因此 450 的局部半径最终显示为约 45 像素。、
        rock->GetGdSprite()->SetColliderCircle(XVec2{0.0f, 0.0f}, 450.0f);
        rock->GetGdSprite()->SetCollisionLayer(2);
        rock->GetGdSprite()->SetCollisionMask(1);

        rock->GetGdSprite()->SetTriggerCircle(XVec2{0.0f, 0.0f}, 450.0f);
        rock->GetGdSprite()->SetTriggerLayer(2);
        rock->GetGdSprite()->SetTriggerMask(1);

        mData->sprites.push_back(rock);
    }

    // 设置背景
    {
        auto background = makeShareObject<Sprite>("background");
        background->CreateGdBackdrop();

        // 设置纹理
        background->GetGdSprite()->SetTexture("res://assets/background.png");
        background->GetGdSprite()->SetZIndex(-1);
        // viewport 960 540
        // 图片2400 1600
        // background->GetGdSprite()->SetPosition(XVec2{480,-360});
        mData->sprites.push_back(background);

        // 设置纹理位置，是的背景和viewport对齐
    }
}
void Game::OnEngineUpdate(float ddelta)
{
    (void)ddelta;

    auto player = GetSprite("player");
    if (!player || !player->GetGdSprite())
    {
        return;
    }
#if 0
    XVec2 direction{};
    if (mData->move_left)
    {
        direction.x -= 1.0f;
    }
    if (mData->move_right)
    {
        direction.x += 1.0f;
    }
    if (mData->move_up)
    {
        direction.y += 1.0f;
    }
    if (mData->move_down)
    {
        direction.y -= 1.0f;
    }

    // 对角线移动时归一化方向，避免速度变成约 1.414 倍。
    const float length = std::sqrt(
        direction.x * direction.x + direction.y * direction.y);
    constexpr float player_speed = 200.0f;
    XVec2 velocity{};
    if (length > 0.0f)
    {
        velocity.x = direction.x / length * player_speed;
        velocity.y = direction.y / length * player_speed;
    }

    auto gd_sprite = player->GetGdSprite();
    gd_sprite->SetVelocity(velocity);
#endif
    // 相机跟随物理结算后的实际位置，碰撞时不会继续穿过障碍物。
    auto gd_sprite = player->GetGdSprite();
    GetCamera()->SetPosition(gd_sprite->GetPosition());
}

void Game::OnKeyPressed(int64_t key)
{
    // 键盘摁下
    //std::cout << "OnKeyPressed:" << key << std::endl;
    switch (static_cast<Keys>(key))
    {
    case Keys::left:
        mData->move_left = true;
        break;
    case Keys::right:
        mData->move_right = true;
        break;
    case Keys::up:
        mData->move_up = true;
        break;
    case Keys::down:
        mData->move_down = true;
        break;
    }
}

void Game::OnKeyReleased(int64_t key)
{
    // 键盘释放
    //std::cout << "OnKeyReleased:" << key << std::endl;
    switch (static_cast<Keys>(key))
    {
    case Keys::left:
        mData->move_left = false;
        break;
    case Keys::right:
        mData->move_right = false;
        break;
    case Keys::up:
        mData->move_up = false;
        break;
    case Keys::down:
        mData->move_down = false;
        break;
    }
}

void Game::OnMousePressed(int64_t key)
{
    std::cout<<"OnMousePressed"<<std::endl;
}

void Game::OnMouseReleased(int64_t key)
{
    std::cout<<"OnMouseReleased"<<std::endl;
}

void Game::OnEngineFixedUpdate(float delta)
{
    auto player = GetSprite("player");
    if (!player || !player->GetGdSprite())
    {
        return;
    }

    XVec2 force{};

    if (mData->move_up)
    {
        force.y += 1500.0f;
    }
    if (mData->move_down)
    {
        force.y -= 1500.0f;
    }

    if (mData->move_left)
    {
        force.x -= 800.0f;
    }

    if (mData->move_right)
    {
        force.x += 800.0f;
    }

    // SPX 侧的 AddImpulse 会在当前动态物理帧消费并清零。
    // 每个固定帧提交 force * delta，等价于持续施加 force，
    // 同时避免使用当前不会自动清零的 external_forces。
    if (force.x != 0.0f || force.y != 0.0f)
    {
        player->GetGdSprite()->AddImpulse(
            XVec2{force.x * delta, force.y * delta});
    }
}

void Game::OnSpriteCollisionEnter(Object_t src, Object_t dest)
{
    auto src_sprite = GetSprite(src);
    auto dest_sprite = GetSprite(dest);
    if(src_sprite &&dest_sprite){
        std::cout<<"collisionEnter:"<<src_sprite->GetName()<<" "<<dest_sprite->GetName()<<std::endl;
    }
}

void Game::OnSpriteTriggerEnter(Object_t src, Object_t dest)
{
    auto src_sprite = GetSprite(src);
    auto dest_sprite = GetSprite(dest);
    if(src_sprite &&dest_sprite){
        std::cout<<"triggerEnter:"<<src_sprite->GetName()<<" "<<dest_sprite->GetName()<<std::endl;
    }
}

void Game::OnSpriteTriggerExited(Object_t src, Object_t dest)
{
    auto src_sprite = GetSprite(src);
    auto dest_sprite = GetSprite(dest);
    if(src_sprite &&dest_sprite){
        std::cout<<"triggerExited:"<<src_sprite->GetName()<<" "<<dest_sprite->GetName()<<std::endl;
    }
}

void Game::OnSpriteScreenEntered(Object_t sprite_id)
{
    std::cout<<"OnSpriteScreenEntered:"<<sprite_id<<std::endl;
    if(auto s = GetSprite(sprite_id)){
        std::cout<<"sprite name:"<< s->GetName()<<std::endl;
    }
}

void Game::OnSpriteScreenExited(Object_t sprite_id)
{
    std::cout<<"OnSpriteScreenExited:"<<sprite_id<<std::endl;
    if(auto s = GetSprite(sprite_id)){
        std::cout<<"sprite name:"<< s->GetName()<<std::endl;
    }
}

sptr<ISprite> Game::GetSprite(const std::string &name)
{
    for (auto &s : mData->sprites)
    {
        if (s->GetName() == name)
        {
            return s;
        }
    }
    return nullptr;
}

sptr<ISprite> Game::GetSprite(Object_t id)
{
    for (auto &s : mData->sprites)
    {
        if (s->GetGdSprite()->GetID() == id)
        {
            return s;
        }
    }
    return nullptr;
}

sptr<ICamera> Game::GetCamera() const
{
    return mData->camera;
}
