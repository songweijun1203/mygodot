#include "DataManger.h"
#include "manger/SpriteMgr.h"

#include "engine/Engine.h"

static bool g_isInited = false;

void DataManger::init()
{
    if (!g_isInited)
    {
        g_isInited = true;
        m_spriteMgr = makeShareObject<SpriteMgr>();
        m_engine = makeShareObject<Engine>();
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

sptr<IEngine> DataManger::getEngine()
{
    return m_engine;
}
