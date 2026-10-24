#include "UiSay.h"
#include "DataManger.h"
#include "manger/UiMgr.h"
#include "GdUiNode.h"

class UiSay::Internal{
public:
    sptr<IGdUiNode> uiNode;
    UiSayVariantNodes nodes;
};

UiSay::UiSay():mData(new Internal)
{
}

UiSay::~UiSay()
{
}

void UiSay::BindVariantNodes()
{
    auto bindFn = [this](const std::string &path) -> std::optional<Object_t>
    {
        if (path == "")
        {
            return std::nullopt;
        }
        auto uiMgr = DataManger::getInstance().getUiMgr()->asDerived<UiMgr>();
        return uiMgr->BindNode(GetUiNode()->GetId(), path);
    };

    auto left_box_id = bindFn("VL");
    auto left_label_id = bindFn("VL/BG/Label");
    auto right_box_id = bindFn("VR");
    auto right_label_id = bindFn("VR/BG/Label");

    auto left_think_box_id = bindFn("VLThink");
    auto left_think_label_id = bindFn("VLThink/BG/MC/Label");
    auto right_think_box_id = bindFn("VRThink");
    auto right_think_label_id = bindFn("VRThink/BG/MC/Label");

    if (left_box_id)
    {
        auto node = makeShareObject<GdUiNode>();
        node->SetId(*left_box_id);
        mData->nodes.left.vbox = node;
    }

    if (left_label_id)
    {
        auto node = makeShareObject<GdUiNode>();
        node->SetId(*left_label_id);
        mData->nodes.left.label = node;
    }

    if (right_box_id)
    {
        auto node = makeShareObject<GdUiNode>();
        node->SetId(*right_box_id);
        mData->nodes.right.vbox = node;
    }

    if (right_label_id)
    {
        auto node = makeShareObject<GdUiNode>();
        node->SetId(*right_label_id);
        mData->nodes.right.label = node;
    }

    if (left_think_label_id)
    {
        auto node = makeShareObject<GdUiNode>();
        node->SetId(*left_think_label_id);
        mData->nodes.leftthink.label = node;
    }

    if (left_think_box_id)
    {
        auto node = makeShareObject<GdUiNode>();
        node->SetId(*left_think_box_id);
        mData->nodes.leftthink.vbox = node;
    }

    if (right_think_label_id)
    {
        auto node = makeShareObject<GdUiNode>();
        node->SetId(*right_think_label_id);
        mData->nodes.rightthink.label = node;
    }

    if (right_think_box_id)
    {
        auto node = makeShareObject<GdUiNode>();
        node->SetId(*right_think_box_id);
        mData->nodes.rightthink.vbox = node;
    }
}

sptr<IGdUiNode> UiSay::GetUiNode() const
{
    return mData->uiNode;
}

bool UiSay::CreateGdNode(const std::string &ui_path)
{
     auto uiMgr = DataManger::getInstance().getUiMgr()->asDerived<UiMgr>();
    if (!uiMgr)
        return false;
    auto id = uiMgr->CreateUiNode(ui_path);
    mData->uiNode = makeShareObject<GdUiNode>();
    mData->uiNode->SetId(id);
    return id != 0;
}

void UiSay::SetScale(const XVec2 &s)
{
}

sptr<IGdUiNode> UiSay::GetChildUiNode(UiSayLeftOrRight type1,  UiSayOrThink type2) const
{
    if(type2 == UiSayOrThink::say){
        if(type1 == UiSayLeftOrRight::left){
            return mData->nodes.left.label;
        }else{
             return mData->nodes.right.label;
        }
    }

    if(type2 == UiSayOrThink::think){
        if(type1 == UiSayLeftOrRight::left){
            return mData->nodes.leftthink.label;
        }else{
             return mData->nodes.rightthink.label;
        }
    }
    return nullptr;
}

sptr<IGdUiNode> UiSay::GetVariantRoot(UiSayLeftOrRight type1, UiSayOrThink type2) const
{
    if(type2 == UiSayOrThink::say){
        if(type1 == UiSayLeftOrRight::left){
            return mData->nodes.left.vbox;
        }else{
             return mData->nodes.right.vbox;
        }
    }

    if(type2 == UiSayOrThink::think){
        if(type1 == UiSayLeftOrRight::left){
            return mData->nodes.leftthink.vbox;
        }else{
             return mData->nodes.rightthink.vbox;
        }
    }
    return nullptr;
}
