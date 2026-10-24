#pragma once
#include "base/IManger.h"
#include "base/IEngine.h"

// 全局数据管理类，管理所有的静态数据对象，可以理解为全局的单例对象管理器
class DataManger
{
public:
    static DataManger &getInstance()
    {
        static DataManger instance;
        return instance;
    }

public:
    void init();
    void release();
    // 获取精灵管理器
    sptr<IManger> getSpriteMgr();
    sptr<IManger> getUiMgr();
    sptr<IManger> getPenMgr();
    sptr<IManger> getAudioMgr();
    sptr<IEngine> getEngine();

private:
    DataManger() = default;
    ~DataManger() = default;
    DataManger(const DataManger &) = delete;
    DataManger &operator=(const DataManger &) = delete;

private:
    sptr<IManger> m_spriteMgr;
    sptr<IManger> m_uiMgr;
    sptr<IManger> m_penMgr;
    sptr<IManger> m_audioMgr;
    sptr<IEngine> m_engine;
};