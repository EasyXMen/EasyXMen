/*******************************************************************************
**                                                                            **
** Copyright (C) iSOFT   (2016-2023)                                          **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to iSOFT.         **
** Passing on and copying of this document, and communication                 **
** of its contents is not permitted without prior written authorization.      **
**                                                                            **
********************************************************************************
**                                                                            **
**  FILENAME    : Os_Hal_Compiler.h                                           **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : functions and operations related to the processor           **
**  COMPILER    : hightec                                                     **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r23                         **
**  Version :   AUTOSAR classic Platform R23--Function Safety                 **
**                                                                            **
*******************************************************************************/

/* The currently supported compiler is hightec */

#ifndef OS_HAL_COMPILER_H
#define OS_HAL_COMPILER_H

/*=======[I N C L U D E S]====================================================*/

/*=======[M A C R O S]========================================================*/

#define OS_ASM __asm volatile

#ifndef OS_EXTERN_ASM
#define OS_EXTERN_ASM EXTERN
#endif

#ifndef OS_GLOBAL_ASM
#define OS_GLOBAL_ASM PUBLIC
#endif

#ifndef OS_WEAK_GLOBAL_ASM
#define OS_WEAK_GLOBAL_ASM PUBWEAK
#endif

#ifndef OS_LOCAL_INLINE
#define OS_LOCAL_INLINE static
#endif

#ifndef OS_LOCAL
#define OS_LOCAL static
#endif

#ifndef OS_ALWAYS_INLINE
#define OS_ALWAYS_INLINE __attribute__((always_inline))
#endif

#ifndef OS_ALIGN
#define OS_ALIGN(bytes) __attribute__((aligned(bytes)))
#endif

#ifndef OS_NAKED
#define OS_NAKED __naked
#endif

#define OS_BEGIN_NO_OPTIMIZE _Pragma("optimize=none")
#define OS_END_NO_OPTIMIZE

#define OS_LINKER_SYMBOL_VMA_START(RegionName) RegionName##$$Base
#define OS_LINKER_SYMBOL_VMA_END(RegionName) RegionName##$$Limit

#define Os_LINKER_SYMBOL_LMA_START(RegionName) RegionName##_INIT$$Base
#define Os_LINKER_SYMBOL_LMA_END(RegionName) RegionName##_INIT$$Limit

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/

#endif /* OS_HAL_COMPILER_H */

/*=======[E N D   O F   F I L E]==============================================*/
