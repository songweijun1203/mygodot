#pragma once

#include "base/IUiComponent.h"

struct SayNodes{
    sptr<IGdUiNode> vbox{nullptr};   
    sptr<IGdUiNode> label{nullptr};
};

struct UiSayVariantNodes{
    SayNodes left;  
    SayNodes right;  
    SayNodes leftthink;  
    SayNodes rightthink;  
};

enum class UiSayOrThink{
    say,
    think
};

enum class UiSayLeftOrRight{
    left,
    right
};

class UiSay:public IUiComponent{
public:
    UiSay();
    ~UiSay();
    void BindVariantNodes() override;
    sptr<IGdUiNode> GetUiNode() const override;
    bool CreateGdNode(const std::string& ui_path) override;

    sptr<IGdUiNode> GetChildUiNode(UiSayLeftOrRight type1,  UiSayOrThink type2) const;
    sptr<IGdUiNode> GetVariantRoot(UiSayLeftOrRight type1,  UiSayOrThink type2) const;
    
private:
    class Internal;
    std::unique_ptr<Internal> mData;
};
