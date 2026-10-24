#include "QuoteBubble.h"
#include "UiQuote.h"

class QuotoBubble::Internal{
public:
    sptr<UiQuote> quote;
    wptr<ISprite> sprite;
};

QuotoBubble::QuotoBubble():mData(new Internal)
{
}

QuotoBubble::~QuotoBubble()
{
}

sptr<IUiComponent> QuotoBubble::GetUiCompnonent() const
{
    return mData->quote;
}

void QuotoBubble::CreateUiCompnonent()
{
    mData->quote = makeShareObject<UiQuote>();
    mData->quote->CreateGdNode("res://engine/ui/UiQuote.tscn");
    mData->quote->BindVariantNodes();
    auto ui_quote = mData->quote;
      auto msg = ui_quote->GetChildUiNode(UiQuoteNodeType::labelMsg);
            auto des = ui_quote->GetChildUiNode(UiQuoteNodeType::labelDes);
            msg->SetText("quto msg");
            des->SetText("sprite quto des");
            ui_quote->GetUiNode()->SetPosition(XVec2{100,100});

    //临时测试
}

void QuotoBubble::SetSprite(sptr<ISprite> sprite)
{
    mData->sprite = sprite;
}

sptr<ISprite> QuotoBubble::GetSprite() const
{
    return mData->sprite.lock();
}
