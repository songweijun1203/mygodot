#pragma once

#include "spx_api.h"
#include "mytype.h"

#include <string>

namespace xutils
{
    inline spx_api::GdString ToGdString(const std::string &str)
    {
        // GdString is a borrowed UTF-8 pointer for SPX input parameters.
        return str.c_str();
    }

    inline spx_api::GdVec2 ToGdVec2(const XVec2 &pos)
    {
        spx_api::GdVec2 a;
        a.x = pos.x;
        a.y = pos.y;
        return a;
    }

    inline XVec2 ToXVec2(const spx_api::GdVec2 &value)
    {
        return XVec2{value.x, value.y};
    }

} // namespace xutils
