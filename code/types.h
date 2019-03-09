#if !defined(types_h)
#define types_h
#include <stdint.h>
#include <float.h>
typedef int8_t      s8;
typedef uint8_t     u8;
typedef int16_t     s16;
typedef uint16_t        u16;
typedef int32_t     s32;
typedef uint32_t        u32;
typedef int64_t     s64;
typedef uint64_t        u64;
typedef intptr_t        smo;
typedef uintptr_t        umo;
typedef float r32;//fix this.
typedef double r64; 

typedef double r64; 

#define KB *1024ll
#define MB *1024ll*1024ll
#define GB *1024ll*1024ll*1024ll
#define TB *1024ll*1024ll*1024ll*1024ll

#define maximum_s32 INT32_MAX
#define maximum_u32 UINT32_MAX

#define maximum_r32 FLT_MAX
#define minimum_r32 -FLT_MAX


#endif
