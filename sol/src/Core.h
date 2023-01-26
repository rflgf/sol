#pragma once

#include "Log.h"

#define SOL_BIT(x) (1 << x)

#include <debug-trap.h>
#define SOL_ASSERT(x, ...)                                                     \
	{                                                                          \
		if (!(x))                                                              \
		{                                                                      \
			SOL_ERROR(__VA_ARGS__);                                            \
			psnip_trap();                                                      \
		}                                                                      \
	}

#define SOL_CORE_ASSERT(x, ...)                                                \
	{                                                                          \
		if (!(x))                                                              \
		{                                                                      \
			SOL_CORE_ERROR(__VA_ARGS__);                                       \
			psnip_trap();                                                      \
		}                                                                      \
	}