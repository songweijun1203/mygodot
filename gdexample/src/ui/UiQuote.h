#pragma once

#include "base/IUiComponent.h"

enum class UiQuoteNodeType{
    container,
    imageL,
    imageR,
    labelDes,
    labelMsg
};

struct UiQuoteVariantNodes{
    sptr<IGdUiNode> container{nullptr};   
    sptr<IGdUiNode> imageL{nullptr};   
    sptr<IGdUiNode> imageR{nullptr};   
    sptr<IGdUiNode> labelDes{nullptr};   
    sptr<IGdUiNode> labelMsg{nullptr};   
};

class UiQuote:public IUiComponent{
public:
    UiQuote();
    ~UiQuote();
    bool CreateGdNode(const std::string& ui_path) override;
    void BindVariantNodes() override;
    sptr<IGdUiNode> GetUiNode() const override;
    void SetScale(const XVec2& s) override;
    sptr<IGdUiNode> GetChildUiNode(UiQuoteNodeType type) const;
private:
    class Internal;
    std::unique_ptr<Internal> mData;
};
