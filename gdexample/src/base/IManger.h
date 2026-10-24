#pragma once

#include "DataObject.h"

class IManger: public XBaseObject{
public:
    virtual ~IManger() = default;
    virtual void OnStart() = 0;
    virtual void OnFixedUpdate(float delta) = 0;
    virtual void OnUpdate() = 0;
    virtual void OnDestroy() = 0;
    virtual void OnPause(bool isPaused) = 0;
};

class BaseManger: public IManger{
public:
    virtual ~BaseManger() = default;
    virtual void OnStart() override {}
    virtual void OnFixedUpdate(float delta) override {}
    virtual void OnUpdate() override {}
    virtual void OnDestroy() override {}
    virtual void OnPause(bool isPaused) override {}
};