#include "Engine.h"
#include "gdexample.h"
#include "spx_api.h"

#include <godot_cpp/classes/node2d.hpp>

#include "game/Game.h"

void Engine::OnEngineStart(godot::Node *SpxEngineNode)
{
    if(!GetGame()){
        //游戏尚未启动，需要启动游戏 todo
        auto g = makeShareObject<Game>();
        SetGame(g);
    }

    GetGame()->OnEngineStart(SpxEngineNode);
}

void Engine::OnEngineUpdate(float delta)
{
    if(auto game = GetGame()){
        game->OnEngineUpdate(delta);
    }
}

void Engine::OnKeyPressed(int64_t key)
{
    if(auto game = GetGame()){
        game->OnKeyPressed(key);
    }
}

void Engine::OnKeyReleased(int64_t key)
{
    if(auto game = GetGame()){
        game->OnKeyReleased(key);
    }
}
