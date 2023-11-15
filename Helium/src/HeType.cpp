// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "HeType.h"

#include "common.h"

#ifdef _DEBUG

std::string getHeTypeStr(HeType type)
{
    switch (type)
    {
    case HeType::I32:
        return "I32";
        break;
    }

    HE_DEBUG_BREAK
    return ERR_STR;
}

#endif
