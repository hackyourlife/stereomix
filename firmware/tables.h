#ifndef __TABLES_H__
#define __TABLES_H__

#include "types.h"

extern const u8 sin4table[256];	/* sin(i / 256.0 * M_PI * 0.5) */
extern const u8 logtable[64];	/* log10(1.0 + (i / 64.0 * 9.0)) */

#endif
