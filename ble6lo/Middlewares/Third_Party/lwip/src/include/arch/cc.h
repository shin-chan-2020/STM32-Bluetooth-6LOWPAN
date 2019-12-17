//arch/cc.h
#ifndef __ARCH_CC_H__
#define __ARCH_CC_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
//#include <sys/time.h>

// Includes definition of mch_printf macro to do printf
#include "mch.h"

#define mch_printf(message,...) {char tmpbuf[512];sprintf(tmpbuf,(char *)message,##__VA_ARGS__);printf(tmpbuf);printf("\r\n");}

typedef int sys_prot_t;  //

#define BYTE_ORDER  LITTLE_ENDIAN
//
//typedef uint8_t     u8_t;
//typedef int8_t      s8_t;
//typedef uint16_t    u16_t;
//typedef int16_t     s16_t;
//typedef uint32_t    u32_t;
//typedef int32_t     s32_t;

//typedef uintptr_t   mem_ptr_t;

#define LWIP_ERR_T  int

/*
printf fundmental:
 1. the printf format arguments specifying field width, or precision, or both
 2. A conversion specifier character (d,i,o,u,x,f,e,g,a,c,s,p,n and %) specifies the type of conversion to be applied on arguments.
    Conversion specification results in fetching zero or more subsequent arguments, converting them, if applicable,
    according to the corresponding conversion specifier, and then writing the result to the output stream.
 3. Each conversion specification is introduced by the character %. After the %, the following appear in sequence:
    - Zero or more flags (in any order) that modify the meaning of the conversion specification.
    - An optional minimum field width
    - An optional precision that gives the minimum number of digits to appear
    - An optional length modifier that specifies the size of the argument
    - Aconversion specifier character that specifies the type of conversion to be applied


*/

/* Define (sn)printf formatters for these lwIP types */
#define U16_F "hu"  /* h: is a length modifier, specifies that the conversion specifier (ex. d, u, x ) following 'h'
                          applies to a short int or unsigned short int argument
                          (the argument will have been promoted according to the integer promotions,
                           but its value shall be converted to short int or unsigned short int before printing)*/
#define S16_F "hd"
#define X16_F "hx"
#define U32_F "u"   /* u: int unsigned     decimal number. */
#define S32_F "d"   /* d: int   signed     decimal number. */
#define X32_F "x"   /* x: int unsigned hexadecimal number */

///* Compiler hints for packing structures */
//#define PACK_STRUCT_FIELD(x)    x
//#define PACK_STRUCT_STRUCT  __attribute__((packed))
//#define PACK_STRUCT_BEGIN
//#define PACK_STRUCT_END


/* define compiler specific symbols */
#if defined (__ICCARM__)

#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x) x
#define PACK_STRUCT_USE_INCLUDES

#elif defined (__CC_ARM)

#define PACK_STRUCT_BEGIN __packed
#define PACK_STRUCT_STRUCT
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x) x


#elif defined (__GNUC__)

#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT __attribute__ ((__packed__))
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x) x

#elif defined (__TASKING__)

#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x) x

#endif


/* Plaform specific diagnostic output */
#define LWIP_PLATFORM_DIAG(x)   do {                \
        mch_printf x;                   \
    } while (0)

#define LWIP_PLATFORM_ASSERT(x) do {                \
        mch_printf("Assert \"%s\" failed at line %d in %s\n",   \
                x, __LINE__, __FILE__);             \
/*        mch_abort(); */                       \
    } while (0)

#endif /* __ARCH_CC_H__ */

      //  : commented above mch_abort to remove warning. just shortcut