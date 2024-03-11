#pragma once

#define CONVERT_RGBA_TO_ARGB(value) ((value & 0xFF000000) >> 8) | ((value & 0x00FF0000) >> 8) | ((value & 0x0000FF00) >> 8) | ((value & 0x000000FF) << 24)

#define SWAP_ENDIANESS32(n) ((n>>24) & 0xff) | ((n << 8) & 0xff0000) |((n >> 8) & 0xff00) | ((n << 24) & 0xff000000);