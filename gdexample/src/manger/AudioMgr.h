#include "base/IManger.h"

class AudioMgr:public BaseManger{
public:
    /// @brief 创建一个音频组
    /// @return 
    Object_t CreateAudio();
    void DestroyAudio(Object_t obj);
    Object_t Play(Object_t obj,Object_t owner, const std::string &path,float attenuation ,float maxDistance);
    void SetLoop(Object_t aid, bool loop);
    void Stop(Object_t aid);

    /// @brief 组级音量
    /// @param obj 
    /// @param volume 线性音量 0-1
    void SetVolume(Object_t obj,float volume);
};