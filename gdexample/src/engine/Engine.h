#pragma once
#include "base/IEngine.h"

class Engine: public IEngine{
public:
    void OnEngineStart(godot::Node * SpxEngineNode) override;
    void OnEngineUpdate(float delta) override;
    void OnKeyPressed(int64_t key) override;
    void OnKeyReleased(int64_t key) override;
};
