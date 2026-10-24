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
    virtual void OnMousePressed(int64_t key) =0;
    virtual void OnMouseReleased(int64_t key) =0;

    virtual void OnEngineFixedUpdate(float delta) = 0;
    virtual void OnSpriteCollisionEnter(Object_t src,Object_t dest) =0;
    virtual void OnSpriteTriggerEnter(Object_t src,Object_t dest) =0;
    virtual void OnSpriteTriggerExited(Object_t src,Object_t dest) =0;
    virtual void OnSpriteScreenEntered(Object_t sprite_id) = 0;
    virtual void OnSpriteScreenExited(Object_t sprite_id) = 0;
public:
    void SetGame(sptr<IGame> g);
    sptr<IGame> GetGame();
protected:
    sptr<IGame> game;
};