/**
 * @file report.h    Define interface to reporting infrastructure
 * @brief 
 *    if -DCOUNT is among the compile flags, then certain algorithms 
 *    are enabled to compute the number of comparisons, the number of 
 *    swaps, and the number of comparisons with Null.
 *    
 *    Also exposes some core logic used by reporting throughout.
 * 
 *
 * @author George Heineman
 * @date 6/15/08
 */

#ifndef __REPORT_HPP__
#define __REPORT_HPP__

#include <math.h>
#include <time.h>
#include <sys/time.h>

/**
 * reporting variables to be defined in report.c 
 */
extern long __compTotal;
extern long __compNilTotal;
extern long __swapTotal;

#ifdef COUNT

/** must be ++ so we can squeeze this into conditionals; see problem.h */
#define ADD_COMP      ++__compTotal
#define ADD_SWAP      ++__swapTotal
#define ADD_NIL_COMP  ++__compNilTotal

#define SWAP_COUNT __swapTotal
#define NIL_COUNT  __compNilTotal
#define COMP_COUNT __compTotal

#endif /* COUNT */

extern int verbose;
extern int numElements;

/** This code is used by both C and C++. */
#ifdef __cplusplus
extern "C" long diffTimer (struct timeval *before, struct timeval *after);
extern "C" void printDiffTimer (long usecs);
extern "C" char *timingString (long usecs);
extern "C" void buildTable(long** times, int R, int C);
extern "C" void port_gettime(struct timespec *ts);
extern "C" long diffNanoTimer(struct timespec *before, struct timespec *after);
#else
extern long diffTimer (struct timeval *before, struct timeval *after);
extern void printDiffTimer (long usecs);
extern char *timingString (long usecs);
extern void buildTable(long** times, int R, int C);
extern void port_gettime(struct timespec *ts);
extern long diffNanoTimer(struct timespec *before, struct timespec *after);
#endif

#endif /* _REPORT_H */
