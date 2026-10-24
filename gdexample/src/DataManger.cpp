#include "DataManger.h"
#include "manger/SpriteMgr.h"
#include "manger/UiMgr.h"
#include "manger/PenMgr.h"
#include "manger/AudioMgr.h"

#include "engine/Engine.h"

static bool g_isInited = false;

void DataManger::init()
{
    if (!g_isInited)
    {
        g_isInited = true;
        m_spriteMgr = makeShareObject<SpriteMgr>();
        m_uiMgr = makeShareObject<UiMgr>();
        m_audioMgr = makeShareObject<AudioMgr>();
        m_engine = makeShareObject<Engine>();
        m_penMgr = makeShareObject<PenMgr>();
    }
}

void DataManger::release()
{
    if (g_isInited)
    {
        g_isInited = false;
        m_spriteMgr.reset();
    }
}

sptr<IManger> DataManger::getSpriteMgr()
{
    return m_spriteMgr;
}

sptr<IManger> DataManger::getUiMgr()
{
    return m_uiMgr;
}

sptr<IManger> DataManger::getPenMgr()
{
    return m_penMgr;
}

sptr<IManger> DataManger::getAudioMgr()
{
    return m_audioMgr;
}

sptr<IEngine> DataManger::getEngine()
{
    return m_engine;
}
