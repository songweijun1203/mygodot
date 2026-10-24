#include "GdUiNode.h"

#include "manger/UiMgr.h"
#include "DataManger.h"

void GdUiNode::SetVisible(bool visible)
{
    auto uiMgr = DataManger::getInstance().getUiMgr()->asDerived<UiMgr>();
    uiMgr->SetVisible(GetId(), visible);
}

void GdUiNode::SetText(const std::string &text)
{
    auto uiMgr = DataManger::getInstance().getUiMgr()->asDerived<UiMgr>();
    uiMgr->SetText(GetId(),text);
}

void GdUiNode::SetScale(const XVec2 &scale)
{
    auto uiMgr = DataManger::getInstance().getUiMgr()->asDerived<UiMgr>();
    uiMgr->SetScale(GetId(),scale);
}

XVec2 GdUiNode::GetScale() const
{
    auto uiMgr = DataManger::getInstance().getUiMgr()->asDerived<UiMgr>();
    return uiMgr->GetScale(GetId());
}

XVec2 GdUiNode::GetCanvasScale() const
{
    auto uiMgr = DataManger::getInstance().getUiMgr()->asDerived<UiMgr>();
    return uiMgr->GetCanvasScale(GetId());
}

float GdUiNode::SetTextMaxWidth(const std::string &text, float width)
{
    auto uiMgr = DataManger::getInstance().getUiMgr()->asDerived<UiMgr>();
    return uiMgr->SetTextMaxWidth(GetId(),text,width);
}

void GdUiNode::SetPosition(const XVec2 &position)
{
    auto uiMgr = DataManger::getInstance().getUiMgr()->asDerived<UiMgr>();
    uiMgr->SetPosition(GetId(),position);
}

XVec2 GdUiNode::GetPosition() const
{
    auto uiMgr = DataManger::getInstance().getUiMgr()->asDerived<UiMgr>();
    return uiMgr->GetPosition(GetId());
}
