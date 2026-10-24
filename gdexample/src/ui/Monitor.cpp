#include "Monitor.h"
#include "DataManger.h"
#include "manger/UiMgr.h"
#include "GdUiNode.h"

/*
struct MonitorVariantPaths {
	std::string root;               当前变体的根容器
	std::string label;              变量名 Label 显示 Monitor 的名称，例如 score
	std::string value;              数值 Label 显示变量或表达式的值
	std::string colorTarget;        背景或颜色容器 设置 Monitor 的颜色
	std::string input;              输入控件 滑块样式中对应的 HSlider
};
*/
static constexpr int kMonitorVariantCount = static_cast<int>(MonitorVariant::Count);

static const MonitorVariantPaths kMonitorVariantPaths[kMonitorVariantCount] = {
 {
    "BG",
    "BG/H/LabelName",
    "BG/H/C/H/LabelValue",
    "",
    ""
},
{
    "ValueOnly",
    "",
    "ValueOnly/LabelValue",
    "",
    ""
},
{
    "ScratchBG",
    "ScratchBG/H/LabelMargin/LabelName",
    "ScratchBG/H/ValueMargin/C/LabelValue",
    "ScratchBG/H/ValueMargin/C",
    ""
},
{
    "ScratchValueOnly",
    "",
    "ScratchValueOnly/C/LabelValue",
    "ScratchValueOnly/C",
    ""
},
{
    "ScratchList",
    "",
    "",
    "",
    ""
},
{
    "ScratchSlider",
    "ScratchSlider/V/H/LabelMargin/LabelName",
    "ScratchSlider/V/H/ValueMargin/C/LabelValue",
    "ScratchSlider/V/H/ValueMargin/C",
    "ScratchSlider/V/Slider",
}

};

class Monitor::Internal{
public:
    MonitorVariantNodes variant_nodes[kMonitorVariantCount];
    sptr<IGdUiNode> uiNode;
};

Monitor::Monitor():mData(std::make_unique<Internal>())
{
}

Monitor::~Monitor()
{
}

void Monitor::BindVariantNodes()
{
    auto bindFn =[this](const std::string& path)->std::optional <Object_t>{
        if(path ==""){
            return std::nullopt;
        }
        auto uiMgr = DataManger::getInstance().getUiMgr()->asDerived<UiMgr>();
        return uiMgr->BindNode(GetUiNode()->GetId(),path);
    };

    for(int i = 0; i < kMonitorVariantCount; i++){
        auto root_id = bindFn(kMonitorVariantPaths[i].root);
        auto colorTarget_id = bindFn(kMonitorVariantPaths[i].colorTarget);
        auto input_id = bindFn(kMonitorVariantPaths[i].input);
        auto label_id = bindFn(kMonitorVariantPaths[i].label);
        auto value_id = bindFn(kMonitorVariantPaths[i].value);
        if(root_id){
            auto node = makeShareObject<GdUiNode>();
            node->SetId(*root_id);
            mData->variant_nodes[i].root =node;
        }
        if(colorTarget_id){
            auto node = makeShareObject<GdUiNode>();
            node->SetId(*colorTarget_id);
            mData->variant_nodes[i].colorTarget =node;
        }

        if(input_id){
            auto node = makeShareObject<GdUiNode>();
            node->SetId(*input_id);
            mData->variant_nodes[i].input =node;
        }

        if(label_id){
            auto node = makeShareObject<GdUiNode>();
            node->SetId(*label_id);
            mData->variant_nodes[i].label =node;
        }

        if(value_id){
            auto node = makeShareObject<GdUiNode>();
            node->SetId(*value_id);
            mData->variant_nodes[i].value =node;
        }
    }

}

void Monitor::SetScale(const XVec2 &s)
{
    mData->uiNode->SetScale(s);
}

bool Monitor::CreateGdNode(const std::string &ui_path)
{
    auto uiMgr = DataManger::getInstance().getUiMgr()->asDerived<UiMgr>();
    if(!uiMgr)
        return false;
    auto id = uiMgr->CreateUiNode(ui_path);
    mData->uiNode = makeShareObject<GdUiNode>();
    mData->uiNode->SetId(id);
    return id != 0;
}

sptr<IGdUiNode> Monitor::GetUiNode() const
{
    return mData->uiNode;
}

sptr<IGdUiNode> Monitor::GetChildUiNode(MonitorVariant variant, MonitorNodeType type) const
{   
    auto index = (int64_t)variant;
    switch (type)
    {
    case MonitorNodeType::colorTarget:
        return mData->variant_nodes[index].colorTarget;
    case MonitorNodeType::input:
        return mData->variant_nodes[index].input;
    case MonitorNodeType::label:
        return mData->variant_nodes[index].label;
    case MonitorNodeType::root:
        return mData->variant_nodes[index].root;
    case MonitorNodeType::value:
        return mData->variant_nodes[index].value;
    default:
        return nullptr;
    }
}

/*
godot/bin/godot.macos.editor.dev.arm64.spx-debug \
  --editor \
  --path gdexample/project

  python3 gdexample/tools/generate_spx_api.py
*/
