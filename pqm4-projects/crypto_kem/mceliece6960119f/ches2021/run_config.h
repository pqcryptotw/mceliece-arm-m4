#ifndef _RUN_CONFIG_H_
#define _RUN_CONFIG_H_

#if defined(PQM4)
#define _M4_ASM_
//#define _PK_IN_FLASH_
#endif

#if defined(STM32F4)
#define _STM32F4_
#endif

//#define _SUPERCOP_

#define _F_PARAM_

#define GEN_PK_T_WIDTH  22

#endif
