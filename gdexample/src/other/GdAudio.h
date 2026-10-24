#include "gdbase/IGdAudio.h"

class GdAudio:public IGdAudio{
public:
    GdAudio();
    ~GdAudio();

    Object_t GetId() const override;
    void SetId(Object_t id) override;
    void CreateAudio() override;
    void DestroyAudio() override;
    Object_t PlayInWorldCenter(const std::string& path,float attenuation =1,float maxDistance=2000)override;
    Object_t PlayFollowCamera(const std::string& path,float attenuation =1,float maxDistance=2000)override;
    Object_t PlayFollowSprite(const std::string& path,
        sptr<IGdSprite> sprite, 
        float attenuation =1,
        float maxDistance=2000)override;
    virtual void SetLoop(Object_t aid, bool loop)override;
    virtual void Stop(Object_t aid)override;
    virtual void SetVolume(float volume)override;

private:
    class Internal;
    std::unique_ptr<Internal> mData;
};