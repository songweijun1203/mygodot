#pragma once

#include "DataObject.h"
//#include "gdbase/IGdSprite.h"

class IGdSprite;
class IBubbleBase;
class IGdPen;
class IGdAudio;
enum class SpriteType:uint8_t;

class ISprite : public XBaseObject{
protected:
    ISprite(const std::string& n):name(n){}
public:
    virtual ~ISprite() = default;
    //代理精灵
    virtual sptr<IGdSprite> GetOrCreateGdSprite(const SpriteType& type)=0;
    virtual sptr<IGdSprite> GetGdSprite()=0;

    //代理pen组件

    //代理音频组件
    virtual sptr<IGdAudio> GetOrCreateAudio()=0;

    //代理气泡
    virtual sptr<IBubbleBase> GetOrCreateTextBubble()  = 0;

    //代理quote
    virtual sptr<IBubbleBase> GetOrCreateQuoteBubble()  =0;

public:
    std::string GetName() const{
        return name;
    }
protected:
    std::string name;
};
