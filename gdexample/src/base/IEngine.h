#pragma once

#include "DataObject.h"
#include "IGame.h"
namespace godot{
    class Node;
}

class IEngine:public XBaseObject{
public:
    virtual void OnEngineStart(godot::Node * SpxEngineNode) = 0;
    virtual void OnEngineUpdate(float delta) = 0;
    virtual void OnKeyPressed(int64_t key) =0;
    virtual void OnKeyReleased(int64_t key) =0;

    virtual void OnEngineFixedUpdate(float delta) = 0;
public:
    void SetGame(sptr<IGame> g);
    sptr<IGame> GetGame();
protected:
    sptr<IGame> game;
};