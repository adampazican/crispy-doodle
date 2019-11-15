#pragma once

#include <stdint.h>

#define internal static
#define global_variable static
#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))
#define Assert(Expression) if(!(Expression)) {*(volatile int *)0 = 0;}
#define CTAssert(Expr) static_assert(Expr, "Assertion failed: " #Expr)


typedef float r32;
typedef double r64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef i32 bool32;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
