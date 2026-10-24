#include "Game.h"

#include "gdexample.h"
#include "base/ISprite.h"
#include "DataManger.h"
#include "manger/SpriteMgr.h"
#include "sprite/Sprite.h"

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/core/memory.hpp>

#include <cmath>
#include <vector>

#include<iostream>

class Game::Internal
{
public:
    godot::Node *root;
    std::vector<sptr<ISprite>> sprites;

    XVec2 player_position{0.0f, 0.0f};
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

    //设置游戏精灵
    {
        auto player = makeShareObject<Sprite>("player");
        player->CreateGdSprite("", XVec2{0, 0});

        player->GetGdSprite()->SetTexture("res://assets/icon.svg");

        player->GetGdSprite()->SetZIndex(0);

        mData->sprites.push_back(player);
    }

    //设置背景
    {
        auto background = makeShareObject<Sprite>("background");
        background->CreateGdBackdrop();

        // 设置纹理
        background->GetGdSprite()->SetTexture("res://assets/background.png");
        background->GetGdSprite()->SetZIndex(-1);
        mData->sprites.push_back(background);
    }

}
void Game::OnEngineUpdate(float ddelta)
{
    auto player = GetSprite("player");
    if (!player || !player->GetGdSprite()) {
        return;
    }

    XVec2 direction{};
    if (mData->move_left) {
        direction.x -= 1.0f;
    }
    if (mData->move_right) {
        direction.x += 1.0f;
    }
    if (mData->move_up) {
        direction.y += 1.0f;
    }
    if (mData->move_down) {
        direction.y -= 1.0f;
    }

    // 对角线移动时归一化方向，避免速度变成约 1.414 倍。
    const float length = std::sqrt(
            direction.x * direction.x + direction.y * direction.y);
    if (length <= 0.0f) {
        return;
    }

    constexpr float player_speed = 200.0f;
    direction.x /= length;
    direction.y /= length;
    mData->player_position.x += direction.x * player_speed * ddelta;
    mData->player_position.y += direction.y * player_speed * ddelta;

    player->GetGdSprite()->SetPosition(mData->player_position);
}

void Game::OnKeyPressed(int64_t key)
{
    //键盘摁下
    std::cout<<"OnKeyPressed:"<<key<<std::endl;
    switch (static_cast<Keys>(key)) {
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
    //键盘释放
   std::cout<<"OnKeyReleased:"<<key<<std::endl;
   switch (static_cast<Keys>(key)) {
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
