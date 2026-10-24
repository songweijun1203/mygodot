#pragma once

#include "base/IManger.h"
#include<set>

class UiMgr : public BaseManger {
    public:
    Object_t CreateUiNode(const std::string &path);
    Object_t BindNode(Object_t parentId, const std::string &relative_path);
    void SetText(Object_t object_id, const std::string &text);
    float SetTextMaxWidth(Object_t object_id, const std::string &text,float width);
    void SetVisible(Object_t object_id,bool visible);
    void SetFontSize(Object_t object_id,int64_t size);
    void SetScale(Object_t object_id, const XVec2 &scale);
    XVec2 GetScale(Object_t);
    XVec2 GetCanvasScale(Object_t);
    void SetPosition(Object_t object_id,const XVec2& scale);
    XVec2 GetPosition(Object_t object_id) const ;

    bool DeleteUiNode(Object_t object_id) const;
};
