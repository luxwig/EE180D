#include "customio.h"
#include <stdio.h>

void MoType_to_str(MoType t, char* str, int type)
{
    switch (t&_MASK2)
    {
        case WALK:
            strcpy(str, "WALK");
            break;
        case RUN:
            strcpy(str, "RUN");
            break;
        case ASC:
            strcpy(str, "ASC");
            break;
        case DSC:
            strcpy(str, "DSC");
            break;
        case TL:
            strcpy(str, "TL");
            break;
        case TR:
            strcpy(str, "TR");
            break;
        case JUMP:
            strcpy(str, "JUMP");
            break;
        case NONE:
            break;
        default:
            strcpy(str, "UDEF");
    }
    if (type == _IO_ALL)
    {
        char digit[3];
        int len = strlen(str);
        if (t&0xF != 0)
        {
            sprintf(digit, "%d", t&0xF);
            strcpy(str+len, digit);
        }
    }
}

void get_motion_str_from_array(MoType* result, char* str)
{
    if (result[_WALK_RUN_OFFSET] != 0)
        MoType_to_str(result[_WALK_RUN_MOD_OFFSET],
                      str, _IO_ALL);
    if (result[_ASC_DSC_OFFSET] != 0)
        MoType_to_str(result[_ASC_DSC_MOD_OFFSET],
                      str+len(str), _IO_ALL);
    MoType_to_str(result[_TL_TR_OFFSET], str+len(str), _IO_ALL);
    MoType_to_str(result[_JUMP_OFFSET], str+len(str), _IO_ALL):
}


