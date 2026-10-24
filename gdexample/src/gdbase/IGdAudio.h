#pragma once

#include "base/DataObject.h"
#include <vector>
class IGdSprite;
class IGdAudio : public XBaseObject
{
public:
    virtual Object_t GetId() const =0;
    virtual void SetId(Object_t id) = 0;
    virtual void CreateAudio() = 0;
    virtual void DestroyAudio() =0;
    virtual Object_t PlayInWorldCenter(const std::string& path,float attenuation =1,float maxDistance=2000)=0;
    virtual Object_t PlayFollowCamera(const std::string& path,float attenuation =1,float maxDistance=2000)=0;
    virtual Object_t PlayFollowSprite(const std::string& path,sptr<IGdSprite> sprite, float attenuation =1,float maxDistance=2000)=0;
    virtual void SetLoop(Object_t aid, bool loop)=0;
    virtual void Stop(Object_t aid)=0;
    virtual void SetVolume(float volume)=0;
};