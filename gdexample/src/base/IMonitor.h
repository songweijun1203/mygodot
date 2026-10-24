#pragma once

#include "DataObject.h"
#include "IUiComponent.h"
#include "base/IGdUiNode.h"

enum class MonitorNodeType
{
    root,
    label,
    value,
    colorTarget,
    input
};

enum class MonitorVariant {
	Default = 0,
	ValueOnly,
	Scratch,
	ScratchValueOnly,
	List,
	Slider,
	Count
};

class IMonitor : public IUiComponent
{
public:
    virtual sptr<IGdUiNode> GetChildUiNode(MonitorVariant variant, MonitorNodeType type) const = 0;
    virtual void SetScale(const XVec2& s)=0;
};
