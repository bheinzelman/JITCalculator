//  jc.h

#pragma once

#include <cassert>

///////////////////////////////////
// Macros
///////////////////////////////////
#define JC_VERSION_STRING "0.0.0"

#define JC_FAIL() assert(0);

#define JC_FAIL_AND_RETURN_NULL() \
	do {\
		JC_FAIL(); \
		return nullptr;\
	} while (0);

#define JC_ASSERT(e) \
	do {\
		assert(e); \
	} while (0);

