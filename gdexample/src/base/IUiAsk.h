#pragma once

#include "DataObject.h"
#include "IUiComponent.h"
enum class UiAskNodeType
{
    input,
    check,
    askLabel,
    askBody,
};

class IGdUiNode;
class IUiAsk:public IUiComponent{
public:
    virtual sptr<IGdUiNode> GetChildUiNode(UiAskNodeType type) const = 0;
};