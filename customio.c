#include "customio.h"
#include <stdio.h>
#include "string.h"
void MoType_to_str(MoType t, char* str, int type)
{
    switch (t&_MASK_LV1)
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
        case TURNL:
            strcpy(str, "TURNL");
            break;
        case TURNR:
            strcpy(str, "TURNR");
            break;
        case JUMP:
            strcpy(str, "JUMP");
            break;
        case NONE:
            break;
        default:
            strcpy(str, "UDEF");
    }
    if (type&0xF0)
    {
        char digit[3];
        int len = strlen(str);
        if (t&0xF)
        {
            sprintf(digit, "%d", t&0xF);
            strcpy(str+len, digit);
        }
    }
    if (type&0x0F)
        strcpy(str+strlen(str), " ");
}

void get_motion_str_from_array(MoType* result, char* str)
{
    if (result[_WALK_RUN_OFFSET] != 0)
        MoType_to_str(result[_WALK_RUN_MOD_OFFSET],
                      str, _IO_ALL_SPACE);
    if (result[_ASC_DSC_OFFSET] != 0)
        MoType_to_str(result[_ASC_DSC_MOD_OFFSET],
                      str+strlen(str), _IO_ALL_SPACE);
    MoType_to_str(result[_TURNR_TURNL_OFFSET], str+strlen(str), _IO_ALL_SPACE);
    MoType_to_str(result[_JUMP_OFFSET], str+strlen(str), _IO_ALL_SPACE);
}


