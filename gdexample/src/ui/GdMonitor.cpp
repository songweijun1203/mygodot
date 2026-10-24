#include "GdMonitor.h"
#include "DataManger.h"
#include "manger/UiMgr.h"
#include "GdUiNode.h"

/*
struct MonitorViewSpec  {
	std::string root;               当前外观的根容器
	std::string label;              变量名 Label 显示 Monitor 的名称，例如 score
	std::string value;              数值 Label 显示变量或表达式的值
	std::string colorTarget;        背景或颜色容器 设置 Monitor 的颜色
	std::string input;              输入控件 滑块样式中对应的 HSlider
};
*/
static MonitorViewSpec m[6]={
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

class GdMonitor::Internal{
public:
    MonitorView monitorviews[6];
    sptr<IGdUiNode> uiNode;
};

GdMonitor::GdMonitor():mData(std::make_unique<Internal>())
{
}

GdMonitor::~GdMonitor()
{
}

void GdMonitor::BindViews()
{
    auto bindFn =[this](const std::string& path)->std::optional <Object_t>{
        if(path ==""){
            return std::nullopt;
        }
        auto uiMgr = DataManger::getInstance().getUiMgr()->asDerived<UiMgr>();
        return uiMgr->BindNode(GetUiNode()->GetId(),path);
    };

    for(int i=0;i<6;i++){
        auto root_id = bindFn(m[i].root);
        auto colorTarget_id = bindFn(m[i].colorTarget);
        auto input_id = bindFn(m[i].input);
        auto label_id = bindFn(m[i].label);
        auto value_id = bindFn(m[i].value);
        if(root_id){
            auto node = makeShareObject<GdUiNode>();
            node->SetId(*root_id);
            mData->monitorviews[i].root =node;
        }
        if(colorTarget_id){
            auto node = makeShareObject<GdUiNode>();
            node->SetId(*colorTarget_id);
            mData->monitorviews[i].colorTarget =node;
        }

        if(input_id){
            auto node = makeShareObject<GdUiNode>();
            node->SetId(*input_id);
            mData->monitorviews[i].input =node;
        }

        if(label_id){
            auto node = makeShareObject<GdUiNode>();
            node->SetId(*label_id);
            mData->monitorviews[i].label =node;
        }

        if(value_id){
            auto node = makeShareObject<GdUiNode>();
            node->SetId(*value_id);
            mData->monitorviews[i].value =node;
        }
    }

}

void GdMonitor::SetScale(const XVec2 &s)
{
    mData->uiNode->SetScale(s);
}

bool GdMonitor::CreateGdNode(const std::string &ui_path)
{
    auto uiMgr = DataManger::getInstance().getUiMgr()->asDerived<UiMgr>();
    if(!uiMgr)
        return false;
    auto id = uiMgr->CreateUiNode(ui_path);
    mData->uiNode = makeShareObject<GdUiNode>();
    mData->uiNode->SetId(id);
    return id != 0;
}

sptr<IGdUiNode> GdMonitor::GetUiNode() const
{
    return mData->uiNode;
}

sptr<IGdUiNode> GdMonitor::GetChildUiNode(const MonitorAppearance &apperaence, const MonitorViewType &type) const
{   
    auto index =(int64_t)apperaence;
    switch (type)
    {
    case MonitorViewType::coloeTarget:
        return mData->monitorviews[index].root;
    case MonitorViewType::input:
        return mData->monitorviews[index].input;
    case MonitorViewType::label:
        return mData->monitorviews[index].label;
    case MonitorViewType::root:
        return mData->monitorviews[index].root;
    case MonitorViewType::value:
        return mData->monitorviews[index].value;
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
