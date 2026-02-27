#pragma once

#define JE_PLATFORM_BREAK() (__nop(), __debugbreak());

// TODO: export/import, etc
#define JE_API