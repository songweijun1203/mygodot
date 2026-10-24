#pragma once

#include "base/IGdMonitor.h"
#include "base/mytype.h"
#include "base/IGdUiNode.h"
struct MonitorViewSpec  {
	std::string root;       
	std::string label;       
	std::string value;       
	std::string colorTarget;
	std::string input;    
};

struct MonitorView {
	sptr<IGdUiNode> root{nullptr};        
	sptr<IGdUiNode> label{nullptr};
	sptr<IGdUiNode> value{nullptr};
	sptr<IGdUiNode> colorTarget{nullptr};
	sptr<IGdUiNode> input{nullptr};
};

class GdMonitor : public IGdMonitor
{
public:
    GdMonitor();
    ~GdMonitor();
public:
    void BindViews() override;
    void SetScale(const XVec2& s) override;
    bool CreateGdNode(const std::string& ui_path) override;
    sptr<IGdUiNode> GetUiNode() const override;
    sptr<IGdUiNode> GetChildUiNode(const MonitorAppearance& apperaence,const MonitorViewType &type) const override;
private:
    class Internal;
    std::unique_ptr<Internal> mData;
};