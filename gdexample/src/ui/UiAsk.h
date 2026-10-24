#pragma once

#include "base/IUiComponent.h"

struct UiAskVariantNodes{
    sptr<IGdUiNode> input{nullptr};   
    sptr<IGdUiNode> checkBtn{nullptr};   
    sptr<IGdUiNode> askBody{nullptr};   
    sptr<IGdUiNode> askLabel{nullptr};   
};

enum class UiAskNodeType
{
    input,
    check,
    askLabel,
    askBody,
};

class UiAsk:public IUiComponent{
public:
    UiAsk();
    ~UiAsk();
    void BindVariantNodes() override;
    sptr<IGdUiNode> GetUiNode() const override;
    bool CreateGdNode(const std::string& ui_path) override;
    void SetScale(const XVec2& s) override;
    sptr<IGdUiNode> GetChildUiNode(UiAskNodeType type) const;
private:
    class Internal;
    std::unique_ptr<Internal> mData;
};
