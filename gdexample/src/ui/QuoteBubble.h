#pragma once
#include "base/IBubbleBase.h"

class QuotoBubble : public IBubbleBase
{
public:
    QuotoBubble();
    ~QuotoBubble();

public:
    sptr<IUiComponent> GetUiCompnonent() const override;
    void CreateUiCompnonent() override;
    void SetSprite(sptr<ISprite> sprite) override;
    sptr<ISprite> GetSprite() const override;
private:
    class Internal;
    std::unique_ptr<Internal> mData;
};