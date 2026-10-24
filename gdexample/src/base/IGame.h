#pragma once

#include "DataObject.h"

namespace godot{
    class Node;
}

class ISprite;
class IGame:public XBaseObject{
public:
    virtual ~IGame() = default;
    virtual void OnEngineStart(godot::Node * SpxEngineNode)=0;
    virtual void OnEngineUpdate(float ddelta) = 0;
    virtual void OnKeyPressed(int64_t key) =0;
    virtual void OnKeyReleased(int64_t key) =0;

    virtual sptr<ISprite> GetSprite(const std::string& name) =0;
};
