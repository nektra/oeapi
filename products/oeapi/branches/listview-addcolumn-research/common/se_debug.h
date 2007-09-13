/* $Id: se_debug.h,v 1.9 2007/04/26 15:05:23 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#ifndef _SE_DEBUG_H_
#define _SE_DEBUG_H_


#define DEBUG_ERROR 0
#define DEBUG_WARNING 1
#define DEBUG_INFO 2
#define DEBUG_TRACE 4

void nkt_debug_print(int level, LPCTSTR format, ...);
inline void nkt_debug_print_dummy(int, LPCTSTR, ...) {}

#ifdef PRINT_ERRORS
#define debug_print nkt_debug_print
//#define dbgprint(a) nkt_debug_print(DEBUG_ERROR, a)
#else
#define debug_print nkt_debug_print_dummy
//#define dbgprint(a)
#endif 

#endif /* _SE_DEBUG_H_ */
