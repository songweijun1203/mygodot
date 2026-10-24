#include "UiQuote.h"
#include "DataManger.h"
#include "manger/UiMgr.h"
#include "GdUiNode.h"

class UiQuote::Internal
{
public:
    sptr<IGdUiNode> uiNode;
    UiQuoteVariantNodes nodes;
};

UiQuote::UiQuote() : mData(new Internal)
{
}

UiQuote::~UiQuote()
{
}

void UiQuote::BindVariantNodes()
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

    auto container_id = bindFn("C");
    auto imageL_id = bindFn("C/ImageL");
    auto imageR_id = bindFn("C/ImageR");
    auto labelDes_id = bindFn("C/LabelDes");
    auto labelMsg_id = bindFn("C/LabelMsg");

    if (container_id)
    {
        auto node = makeShareObject<GdUiNode>();
        node->SetId(*container_id);
        mData->nodes.container = node;
    }

    if (imageL_id)
    {
        auto node = makeShareObject<GdUiNode>();
        node->SetId(*imageL_id);
        mData->nodes.imageL = node;
    }

    if (imageR_id)
    {
        auto node = makeShareObject<GdUiNode>();
        node->SetId(*imageR_id);
        mData->nodes.imageR = node;
    }

    if (labelDes_id)
    {
        auto node = makeShareObject<GdUiNode>();
        node->SetId(*labelDes_id);
        mData->nodes.labelDes = node;
    }

    if (labelMsg_id)
    {
        auto node = makeShareObject<GdUiNode>();
        node->SetId(*labelMsg_id);
        mData->nodes.labelMsg = node;
    }
}

sptr<IGdUiNode> UiQuote::GetUiNode() const
{
    return mData->uiNode;
}

bool UiQuote::CreateGdNode(const std::string &ui_path)
{
    auto uiMgr = DataManger::getInstance().getUiMgr()->asDerived<UiMgr>();
    if (!uiMgr)
        return false;
    auto id = uiMgr->CreateUiNode(ui_path);
    mData->uiNode = makeShareObject<GdUiNode>();
    mData->uiNode->SetId(id);
    return id != 0;
}

sptr<IGdUiNode> UiQuote::GetChildUiNode(UiQuoteNodeType type) const
{
    switch (type)
    {
    case UiQuoteNodeType::container:
        return mData->nodes.container;
    case UiQuoteNodeType::imageL:
        return mData->nodes.imageL;
    case UiQuoteNodeType::imageR:
        return mData->nodes.imageR;
    case UiQuoteNodeType::labelDes:
        return mData->nodes.labelDes;
    case UiQuoteNodeType::labelMsg:
        return mData->nodes.labelMsg;
    default:
        return nullptr;
    }
}
