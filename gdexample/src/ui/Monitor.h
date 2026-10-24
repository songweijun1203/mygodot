#pragma once

#include "base/IUiComponent.h"
#include "base/mytype.h"
#include "gdbase/IGdUiNode.h"
struct MonitorVariantPaths {
	std::string root;       
	std::string label;       
	std::string value;       
	std::string colorTarget;
	std::string input;    
};

struct MonitorVariantNodes {
	sptr<IGdUiNode> root{nullptr};        
	sptr<IGdUiNode> label{nullptr};
	sptr<IGdUiNode> value{nullptr};
	sptr<IGdUiNode> colorTarget{nullptr};
	sptr<IGdUiNode> input{nullptr};
};

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

class Monitor : public IUiComponent
{
public:
    Monitor();
    ~Monitor();
public:
    void BindVariantNodes() override;
    void SetScale(const XVec2& s) override;
    bool CreateGdNode(const std::string& ui_path) override;
    sptr<IGdUiNode> GetUiNode() const override;
    sptr<IGdUiNode> GetChildUiNode(MonitorVariant variant, MonitorNodeType type) const;
private:
    class Internal;
    std::unique_ptr<Internal> mData;
};
