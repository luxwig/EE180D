#include "global.h"

#define _IO_NAME_ONLY_NO_SPACE 0x00
#define _IO_NAME_ONLY_SPACE    0x0F
#define _IO_ALL_NO_SPACE       0xF0
#define _IO_ALL_SPACE          0xFF


void MoType_to_str(MoType t, char* str, int type);
void get_motion_str_from_array(MoType* result, char* str);

