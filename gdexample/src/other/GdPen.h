
#include "gdbase/IGdPen.h"

class GdPen:public IGdPen{
public:
    GdPen();
    ~GdPen();
public:
    Object_t GetId() const override;
    void SetId(Object_t id) override;
    void CreatePen() override;

    void PenDown() override;  
    void PenMove(const XVec2& pos) override;
    void PenUp() override;
    void SetPenColor(float r,float g,float b,float a) override;
    void SetPenSize(float size) override;
    void DrawPolygons(const std::vector<XVec2>&points) override;
    void EraseAll() override;

    void PenStamp() override;
    void PenStampSprite(sptr<IGdSprite> sprite) override;
private:
    class Internal;
    std::unique_ptr<Internal> mData;
};