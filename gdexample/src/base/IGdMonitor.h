#pragma once

#include "DataObject.h"
#include "base/IGdUiNode.h"

enum class MonitorViewType
{
    root,
    label,
    value,
    coloeTarget,
    input
};

enum class MonitorAppearance{
	MonitorAppearanceDefault =0,
	MonitorAppearanceDefaultLarge,
	MonitorAppearanceCompatible,
	MonitorAppearanceCompatibleLarge,
	MonitorAppearanceList,
	MonitorAppearanceSlider,
	monitorAppearanceCount
};

class IGdMonitor : public XBaseObject
{
public:
    virtual sptr<IGdUiNode> GetUiNode() const = 0;
    virtual sptr<IGdUiNode> GetChildUiNode(const MonitorAppearance& apperaence,const MonitorViewType &type) const = 0;
    virtual void BindViews() =0;
    virtual void SetScale(const XVec2& s)=0;
    virtual bool CreateGdNode(const std::string& ui_path)=0;
};