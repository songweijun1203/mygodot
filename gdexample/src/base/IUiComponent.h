#pragma once

#include "DataObject.h"
#include "base/IGdUiNode.h"

class IUiComponent : public XBaseObject
{
public:
    virtual ~IUiComponent() = default;
    virtual bool CreateGdNode(const std::string& ui_path) = 0;
    virtual sptr<IGdUiNode> GetUiNode() const = 0;
    virtual void BindVariantNodes() =0;
};