#pragma once

#include "base/IGame.h"
#include<memory>

class Game: public IGame{
public:
    Game();
    ~Game() override;
    void OnEngineStart(godot::Node * SpxEngineNode) override;
    void OnEngineUpdate(float ddelta) override;
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

    sptr<ISprite> GetSprite(const std::string& name) override;
    sptr<ISprite> GetSprite(Object_t id) override;
    sptr<ICamera> GetCamera() const override;
protected:
    class Internal;
    std::unique_ptr<Internal> mData;
};
