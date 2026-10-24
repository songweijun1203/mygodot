#pragma once

#include "base/IUiAsk.h"

struct UiAskVariantNodes{
    sptr<IGdUiNode> input{nullptr};   
    sptr<IGdUiNode> checkBtn{nullptr};   
    sptr<IGdUiNode> askBody{nullptr};   
    sptr<IGdUiNode> askLabel{nullptr};   
};

class UiAsk:public IUiAsk{
public:
    UiAsk();
    ~UiAsk();
    void BindVariantNodes() override;
    sptr<IGdUiNode> GetUiNode() const override;
    bool CreateGdNode(const std::string& ui_path) override;
    sptr<IGdUiNode> GetChildUiNode(UiAskNodeType type) const override;
private:
    class Internal;
    std::unique_ptr<Internal> mData;
};
