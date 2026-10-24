#include "ICamera.h"

void ICamera::SetFollowSprite(sptr<ISprite> s)
{
    followSprite = s;
}

sptr<ISprite> ICamera::GetFollowSprite() const
{
    return followSprite.lock();
}
