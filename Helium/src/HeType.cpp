#include "HeType.h"

#include "common.h"

#ifdef _DEBUG

#pragma warning(suppress: HE_RETURN_CHECK)
std::string getHeTypeStr(HeType type)
{
    switch (type)
    {
    case HeType::I32:
        return "I32";
        break;
    }

    HE_DEBUG_BREAK
}

#endif
