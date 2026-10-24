#include "gdbase/IGdUiNode.h"

class GdUiNode:public IGdUiNode{
    public:
    Object_t GetId() const override{
        return id;
    }

    void SetId(Object_t i) override{
        this->id = i;
    }

    void SetVisible(bool visible) override;

    void SetText(const std::string& text) override;

    void SetScale(const XVec2& scale) override;

    XVec2 GetScale()const override;

    XVec2 GetCanvasScale() const override;

    float SetTextMaxWidth(const std::string& text,float width) override;

    void SetPosition(const XVec2& scale) override;
    XVec2 GetPosition() const override;

    virtual bool DeleteLater() override;
public:
    Object_t id;
};