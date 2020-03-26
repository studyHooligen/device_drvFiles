#ifndef _FLASH_MT29F4G08_H_
#define _FLASH_MT29F4G08_H_

#define FLASH29_ReadID                          0x90
#define FLASH29_SetFeature                      0xEF
#define FLASH29_ReSet                           0xFF
#define FLASH29_ReadStatus                      0x70
#define FLASH29_ReadPage_P1                     0x00
#define FLASH29_ReadPage_P2                     0x30
#define FLASH29_WritePage_P1                    0x80
#define FLASH29_WritePage_P2                    0x10
#define FLASH29_EraseBlock_P1                   0x60
#define FLASH29_EraseBlock_P2                   0XD0
#define FLASH29_ReadForInternalDataMove_P1      0x00
#define FLASH29_ReadForInternalDataMove_P2      0X35
#define FLASH29_ProgramForInternalDataMove_P1   0x85
#define FLASH29_ProgramForInternalDataMove_P1   0X10



#endif
