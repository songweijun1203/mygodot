#include "AudioMgr.h"
#include "spx_api.h"

Object_t AudioMgr::CreateAudio()
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
     spx_api::GdObj ret = 0;
    api.audio_create_audio(&ret);
    return ret;
}

void AudioMgr::DestroyAudio(Object_t obj)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    api.audio_destroy_audio(obj);
}

Object_t AudioMgr::Play(Object_t obj,Object_t owner, const std::string &path,float attenuation ,float maxDistance)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    spx_api::GdInt aid = 0;
    api.audio_play_with_attenuation(obj, xutils::ToGdString(path),owner,attenuation,maxDistance,&aid);
    return aid;
}

void AudioMgr::SetLoop(Object_t aid, bool loop)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    api.audio_set_loop(aid, loop);
}

void AudioMgr::Stop(Object_t aid)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    api.audio_stop(aid);
}

void AudioMgr::SetVolume(Object_t obj, float volume)
{
    auto &api = spx_api::GDExtensionInterface::get_instance();
    api.audio_set_volume(obj, volume);
}
