#include "Game.h"

#include "gdexample.h"
#include "base/ISprite.h"
#include "DataManger.h"
#include "manger/SpriteMgr.h"
#include "sprite/Sprite.h"
#include "ui/Monitor.h"
#include "ui/UiAsk.h"
#include "ui/UiSay.h"
#include "ui/UiQuote.h"
#include "camera/Camera.h"
#include "spx_api.h"

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/memory.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/window.hpp>

#include <cmath>
#include <vector>

#include <iostream>

// 临时测试
std::vector<std::string> monitor_test_labels{
    {"SetScratchValueOnlyLabelValueText SetScratchValueOnlyLabelValueText"},
    {"test!"},
    {"helloworld"}};

class Game::Internal
{
public:
    Internal()
    {
        camera = makeShareObject<Camera>();
        viewport_width = 0;
        viewport_height = 0;
    }

public:
    godot::Node *root;
    std::vector<sptr<ISprite>> sprites;
    sptr<Camera> camera;
    sptr<IMonitor> monitor;
    sptr<IUiAsk> uiask;
    sptr<IUiComponent> uiSay;
    sptr<IUiComponent> uiQuote;
    int64_t viewport_width;
    int64_t viewport_height;

    bool move_left = false;
    bool move_right = false;
    bool move_up = false;
    bool move_down = false;

    bool isMonitorInit = false;
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
    {
        godot::Viewport *viewport = nullptr;

        // 启动回调可能早于 SpxEngineNode 完成入树，节点自身的
        // get_viewport() 此时可能暂时返回空指针。
        if (SpxEngineNode != nullptr)
        {
            viewport = SpxEngineNode->get_viewport();

            if (viewport == nullptr)
            {
                if (auto *tree = SpxEngineNode->get_tree(); tree != nullptr)
                {
                    viewport = tree->get_root();
                }
            }
        }
    }

    {
        auto *settings = godot::ProjectSettings::get_singleton();

        const int64_t design_width = static_cast<int64_t>(
            settings->get_setting(
                "display/window/size/viewport_width",
                640));

        const int64_t design_height = static_cast<int64_t>(
            settings->get_setting(
                "display/window/size/viewport_height",
                360));
        mData->viewport_width = design_width;
        mData->viewport_height = design_height;
    }
    // 监视器节点 创建
    {
        mData->monitor = makeShareObject<Monitor>();
        mData->monitor->CreateGdNode("res://engine/ui/UiMonitor.tscn");
        mData->monitor->BindVariantNodes();

        mData->monitor->GetUiNode()->SetVisible(false);
        mData->monitor->GetUiNode()->SetScale(XVec2{3, 3});
    }

    // uiask节点创建
    {
        mData->uiask = makeShareObject<UiAsk>();
        if (mData->uiask->CreateGdNode("res://engine/ui/UiAsk.tscn"))
        {
            mData->uiask->GetUiNode()->SetVisible(false);
            mData->uiask->BindVariantNodes();
            auto ask_body = mData->uiask->GetChildUiNode(UiAskNodeType::askBody);
            if (ask_body)
            {
                ask_body->SetVisible(true);
            }
            auto label_ask = mData->uiask->GetChildUiNode(UiAskNodeType::askLabel);
            label_ask->SetText("2134429530859045646456456456456666666666346734567375543457457347435734575437337354680456");
        }
    }

    // uisay节点创建
    {
        mData->uiSay = makeShareObject<UiSay>();
        if (mData->uiSay->CreateGdNode("res://engine/ui/UiSay.tscn"))
        {
            auto ui_say =mData->uiSay->asDerived<UiSay>();

            ui_say->BindVariantNodes();
            auto leftSay = ui_say->GetChildUiNode(UiSayLeftOrRight::left, UiSayOrThink::say);
            auto leftThink = ui_say->GetChildUiNode(UiSayLeftOrRight::left, UiSayOrThink::think);
            auto left_bubble = ui_say->GetVariantRoot(UiSayLeftOrRight::left,UiSayOrThink::say);
            auto rightSay = ui_say->GetChildUiNode(UiSayLeftOrRight::right, UiSayOrThink::say);
            auto rightThink = ui_say->GetChildUiNode(UiSayLeftOrRight::right, UiSayOrThink::think);
            auto right_bubble = ui_say->GetVariantRoot(UiSayLeftOrRight::right,UiSayOrThink::say);
           
            left_bubble->SetVisible(true);
            right_bubble->SetVisible(true);
           
            leftSay->SetText("left say");
            rightSay->SetText("right say");
            leftThink->SetText("left think");
            rightThink->SetText("right think");

             mData->uiSay->GetUiNode()->SetPosition(XVec2{300, 300});
        }
    }

    //uiquote节点
    {   

        mData->uiQuote = makeShareObject<UiQuote>();
        if (mData->uiQuote->CreateGdNode("res://engine/ui/UiQuote.tscn")){
            auto ui_quote =mData->uiQuote->asDerived<UiQuote>();
            ui_quote->BindVariantNodes();

            auto msg = ui_quote->GetChildUiNode(UiQuoteNodeType::labelMsg);
            auto des = ui_quote->GetChildUiNode(UiQuoteNodeType::labelDes);
            msg->SetText("quto msg");
            des->SetText("quto des");
            ui_quote->GetUiNode()->SetPosition(XVec2{100,100});
        }
    }
    // 使用碰撞形状判断触发器，不根据纹理透明像素判断重叠。
    auto &spx = spx_api::GDExtensionInterface::get_instance();
    if (spx.physics_set_collision_system_type != nullptr)
    {
        spx.physics_set_collision_system_type(false);
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

        // 相机改为死区跟随，人物不再被钉在屏幕正中。
        mData->camera->SetFollowSprite(player);
        // 引擎自带的位置平滑会和 Follow 的阻尼叠加成两段延迟，
        // 平滑交给 Follow 自己做。
        mData->camera->SetCameraSmoothing(false);
        // 这两个值决定手感，改这里就够了。
        mData->camera->SetFollowDeadzone(XVec2{0.5f, 0.35f});
        mData->camera->SetFollowDamping(6.0f);
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
    auto rect = GetCamera()->GetViewportRect();
    auto sss = rect.size;
    std::cout << sss.x << "  " << sss.y << std::endl;
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
    // 死区规则在 ICamera::Follow 里，人物只在死区外才拖着相机走。
    GetCamera()->Follow(ddelta);

    if (mData->isMonitorInit == false)
    {
        auto root = mData->monitor->GetChildUiNode(MonitorVariant::ScratchValueOnly, MonitorNodeType::root);
        root->SetVisible(true);

        auto label = mData->monitor->GetChildUiNode(MonitorVariant::ScratchValueOnly, MonitorNodeType::value);
        auto cavans_scale = label->GetCanvasScale();
        {
            auto ss = root->GetScale(); //(0.45,0.45)
            auto s = label->GetScale();
            auto total_scale_x = 3 * ss.x * s.x;
            auto total_scale_y = 3 * ss.y * s.y;
        }

        auto w = (float)mData->viewport_width / cavans_scale.x;

        auto r_w = label->SetTextMaxWidth("SetScratchValueOnlyLabelValueTextSetScratchValueOnlyLabelValueText", w - 4 - 18);
        mData->isMonitorInit = true;
    }
}

void Game::OnKeyPressed(int64_t key)
{
    // 键盘摁下
    // std::cout << "OnKeyPressed:" << key << std::endl;
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
    case Keys::space:
    {
        // 临时测试
        static int index = 1;
        auto root = mData->monitor->GetChildUiNode(MonitorVariant::ScratchValueOnly, MonitorNodeType::root);
        auto label = mData->monitor->GetChildUiNode(MonitorVariant::ScratchValueOnly, MonitorNodeType::value);
        auto cavans_scale = label->GetCanvasScale();

        auto w = (float)mData->viewport_width / cavans_scale.x;
        index = (index + 1) % 3;
        auto r_w = label->SetTextMaxWidth(monitor_test_labels[index], w - 4 - 18);
    }
    break;
    }
}

void Game::OnKeyReleased(int64_t key)
{
    // 键盘释放
    // std::cout << "OnKeyReleased:" << key << std::endl;
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
    std::cout << "OnMousePressed" << std::endl;
}

void Game::OnMouseReleased(int64_t key)
{
    std::cout << "OnMouseReleased" << std::endl;
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
    if (src_sprite && dest_sprite)
    {
        std::cout << "collisionEnter:" << src_sprite->GetName() << " " << dest_sprite->GetName() << std::endl;
    }
}

void Game::OnSpriteTriggerEnter(Object_t src, Object_t dest)
{
    auto src_sprite = GetSprite(src);
    auto dest_sprite = GetSprite(dest);
    if (src_sprite && dest_sprite)
    {
        std::cout << "triggerEnter:" << src_sprite->GetName() << " " << dest_sprite->GetName() << std::endl;
    }
}

void Game::OnSpriteTriggerExited(Object_t src, Object_t dest)
{
    auto src_sprite = GetSprite(src);
    auto dest_sprite = GetSprite(dest);
    if (src_sprite && dest_sprite)
    {
        std::cout << "triggerExited:" << src_sprite->GetName() << " " << dest_sprite->GetName() << std::endl;
    }
}

void Game::OnSpriteScreenEntered(Object_t sprite_id)
{
    std::cout << "OnSpriteScreenEntered:" << sprite_id << std::endl;
    if (auto s = GetSprite(sprite_id))
    {
        std::cout << "sprite name:" << s->GetName() << std::endl;
    }
}

void Game::OnSpriteScreenExited(Object_t sprite_id)
{
    std::cout << "OnSpriteScreenExited:" << sprite_id << std::endl;
    if (auto s = GetSprite(sprite_id))
    {
        std::cout << "sprite name:" << s->GetName() << std::endl;
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

sptr<IMonitor> Game::GetMonitor() const
{
    return mData->monitor;
}

sptr<IUiAsk> Game::GetUiask() const
{
    return mData->uiask;
}
