#pragma once

#ifdef DEBUG
#define DEBUG_P 1
#else
#define DEBUG_P 0
#endif
/******************************************************************************
 * Passes debug_printf to printf.  If we're not debugging, do not print debug
 * statements to the console.
 *****************************************************************************/
#define debug_printf(fmt, ...) \
		do { if (DEBUG_P) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

