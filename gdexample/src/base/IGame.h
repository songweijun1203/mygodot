#pragma once

#include "DataObject.h"

namespace godot{
    class Node;
}

class ISprite;
class ICamera;
class IUiComponent;
class IGame:public XBaseObject{
public:
    virtual ~IGame() = default;
    virtual void OnEngineStart(godot::Node * SpxEngineNode)=0;
    virtual void OnEngineUpdate(float ddelta) = 0;
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

    virtual void OnUiNodeDestoryed(Object_t sprite_id) =0;

    virtual sptr<ISprite> GetSprite(const std::string& name) =0;
    virtual sptr<ISprite> GetSprite(Object_t id) =0;
    virtual sptr<ICamera> GetCamera() const=0;
    virtual sptr<IUiComponent> GetMonitor() const =0;
    virtual sptr<IUiComponent> GetUiask() const =0;
};
