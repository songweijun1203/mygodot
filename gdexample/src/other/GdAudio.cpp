#include "GdAudio.h"
#include "DataManger.h"
#include "manger/AudioMgr.h"
#include "gdbase/IGdSprite.h"

class GdAudio::Internal{
public:
    Object_t id;
};

GdAudio::GdAudio():mData(new Internal)
{
}

GdAudio::~GdAudio()
{
}

Object_t GdAudio::GetId() const
{
    return mData->id;
}

void GdAudio::SetId(Object_t id)
{
    mData->id = id;
}

void GdAudio::CreateAudio()
{
    auto audio_mgr = DataManger::getInstance().getAudioMgr()->asDerived<AudioMgr>();
    SetId(audio_mgr->CreateAudio());
}

void GdAudio::DestroyAudio()
{
    auto audio_mgr = DataManger::getInstance().getAudioMgr()->asDerived<AudioMgr>();
    audio_mgr->DestroyAudio(GetId());
}

Object_t GdAudio::PlayInWorldCenter(const std::string &path,float attenuation,float maxDistance)
{
    auto audio_mgr = DataManger::getInstance().getAudioMgr()->asDerived<AudioMgr>();
    return audio_mgr->Play(GetId(),0,path,attenuation,maxDistance);
}

Object_t GdAudio::PlayFollowCamera(const std::string &path, float attenuation, float maxDistance)
{
    auto audio_mgr = DataManger::getInstance().getAudioMgr()->asDerived<AudioMgr>();
    return audio_mgr->Play(GetId(),-1,path,attenuation,maxDistance);
}

Object_t GdAudio::PlayFollowSprite(const std::string &path, sptr<IGdSprite> sprite, float attenuation, float maxDistance)
{
    auto audio_mgr = DataManger::getInstance().getAudioMgr()->asDerived<AudioMgr>();
    return audio_mgr->Play(GetId(),sprite->GetID(),path,attenuation,maxDistance);
}

void GdAudio::SetLoop(Object_t aid, bool loop)
{
    auto audio_mgr = DataManger::getInstance().getAudioMgr()->asDerived<AudioMgr>();
    audio_mgr->SetLoop(aid,loop);
}

void GdAudio::Stop(Object_t aid)
{
    auto audio_mgr = DataManger::getInstance().getAudioMgr()->asDerived<AudioMgr>();
    audio_mgr->Stop(aid);
}

void GdAudio::SetVolume(float volume)
{
    auto audio_mgr = DataManger::getInstance().getAudioMgr()->asDerived<AudioMgr>();
    audio_mgr->SetVolume(GetId(),volume);
}
