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
    void OnEngineFixedUpdate(float delta) override;

    sptr<ISprite> GetSprite(const std::string& name) override;
    sptr<ICamera> GetCamera() const override;
protected:
    class Internal;
    std::unique_ptr<Internal> mData;
};
