#include "UiAsk.h"
#include "GdUiNode.h"
#include "DataManger.h"
#include "manger/UiMgr.h"

class UiAsk::Internal
{
public:
    sptr<IGdUiNode> uiNode;
    UiAskVariantNodes nodes;
};

UiAsk::UiAsk() : mData(new Internal)
{
}

UiAsk::~UiAsk()
{
}

void UiAsk::BindVariantNodes()
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

    auto askBody_id = bindFn("MF/Frame/AskBody");
    auto labelask_id = bindFn("MF/Frame/AskBody/LabelAsk");
    auto input_id = bindFn("M/Input");
    auto check_id = bindFn("M/Input/Check");

    if (askBody_id)
    {
        auto node = makeShareObject<GdUiNode>();
        node->SetId(*askBody_id);
        mData->nodes.askBody = node;
    }
    if (labelask_id)
    {
        auto node = makeShareObject<GdUiNode>();
        node->SetId(*labelask_id);
        mData->nodes.askLabel = node;
    }
    if (input_id)
    {
        auto node = makeShareObject<GdUiNode>();
        node->SetId(*input_id);
        mData->nodes.input = node;
    }
    if (check_id)
    {
        auto node = makeShareObject<GdUiNode>();
        node->SetId(*check_id);
        mData->nodes.checkBtn = node;
    }
}

sptr<IGdUiNode> UiAsk::GetUiNode() const
{
    return mData->uiNode;
}

bool UiAsk::CreateGdNode(const std::string &ui_path)
{
    auto uiMgr = DataManger::getInstance().getUiMgr()->asDerived<UiMgr>();
    if (!uiMgr)
        return false;
    auto id = uiMgr->CreateUiNode(ui_path);
    mData->uiNode = makeShareObject<GdUiNode>();
    mData->uiNode->SetId(id);
    return id != 0;
}

void UiAsk::SetScale(const XVec2 &s)
{
}

sptr<IGdUiNode> UiAsk::GetChildUiNode(UiAskNodeType type) const
{
    switch (type)
    {
    case UiAskNodeType::input:
        return mData->nodes.input;
    case UiAskNodeType::askBody:
        return mData->nodes.askBody;
    case UiAskNodeType::askLabel:
        return mData->nodes.askLabel;
    case UiAskNodeType::check:
        return mData->nodes.checkBtn;
    default:
        return nullptr;
    }
}
