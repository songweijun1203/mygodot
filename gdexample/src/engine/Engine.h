#pragma once
#include "base/IEngine.h"

class Engine: public IEngine{
public:
    void OnEngineStart(godot::Node * SpxEngineNode) override;
    void OnEngineUpdate(float delta) override;
    void OnKeyPressed(int64_t key) override;
    void OnKeyReleased(int64_t key) override;
    void OnMousePressed(int64_t key) override;
    void OnMouseReleased(int64_t key) override;
    void OnEngineFixedUpdate(float delta) override;
    void OnSpriteCollisionEnter(Object_t src,Object_t dest) override;
    void OnSpriteTriggerEnter(Object_t src,Object_t dest) override;
    void OnSpriteTriggerExited(Object_t src,Object_t dest) override;

    void OnSpriteScreenEntered(Object_t sprite_id) override;
    void OnSpriteScreenExited(Object_t sprite_id) override;
};
