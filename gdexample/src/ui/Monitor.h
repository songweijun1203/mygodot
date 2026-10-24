#pragma once

#include "base/IMonitor.h"
#include "base/mytype.h"
#include "base/IGdUiNode.h"
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

class Monitor : public IMonitor
{
public:
    Monitor();
    ~Monitor();
public:
    void BindVariantNodes() override;
    void SetScale(const XVec2& s) override;
    bool CreateGdNode(const std::string& ui_path) override;
    sptr<IGdUiNode> GetUiNode() const override;
    sptr<IGdUiNode> GetChildUiNode(MonitorVariant variant, MonitorNodeType type) const override;
private:
    class Internal;
    std::unique_ptr<Internal> mData;
};
