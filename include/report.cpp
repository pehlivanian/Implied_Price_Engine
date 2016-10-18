/**
 * @file Timing/report.c    Common reporting code
 * @brief 
 *    Implements a standard reporting approach used throughout the repository.
 *
 *
 * @author George Heineman
 * @date 6/15/08
 */


#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>

#include "report.hpp"


#ifndef DEFINED_VARS
#define DEFINED_VARS
long __compTotal = 0;
long __compNilTotal = 0;
long __swapTotal = 0;
#endif /* DEFINED_VARS */

/**
 * Compute the difference between two time values in usecs. 
 * \param before   Time before a computation started
 * \param after   Time when a computation completed
 * \return        difference of these two times in long microseconds.
 */
long diffTimer (struct timeval *before, struct timeval *after) {
  long ds = after->tv_sec - before->tv_sec;
  long uds = after->tv_usec - before->tv_usec;

  /* if secs are same, then return simple delta */
  if (ds == 0) {
    return uds;
  }

  /* ok, so we are turned over. Something like: */
  /* before: 1179256745 744597                  */
  /* after:  1179256746 73514                   */

  /* if we have 'turned over' then account properly */
  if (uds < 0) {
    ds = ds - 1;
    uds += 1000000;
  }

  return 1000000*ds + uds;
}

/* more than enough for packing integers. */
static char packed[64];

/**
 * Convert microseconds into string showing seconds and microseconds.
 *
 * \param usecs    absolute time amount in microseconds
 * \return         a char* string representing "N.M" seconds.
 */
char *timingString(long usecs) {
  int i;
  long secs = usecs / 1000000;
  usecs = usecs - 1000000*secs;
  sprintf (packed, "%ld.%6ld secs", secs, usecs);
  for (i = 0; i < 10; i++) {
    /* pad with zeroes. */
    if (packed[i] == ' ') { packed[i] = '0'; }
  }
  return packed;
}

/** 
 * Print the time difference.
 */
void printDiffTimer (long usecs) {
  printf ("%s\n", timingString(usecs));
}

/** Probably enough scratch room. */
char buf[1024];

/**
 * Build a row for the output by computing average and stdev after discarding
 * the lowest and highest results.
 */
char *buildRow(long n, long **times, int T) {
  long sum = 0, min, max;
  int i, ct;
  double mean;
  double calc = 0.;
  int minIdx = 0;
  int maxIdx = 0;

  // Can't help myself

    int exclude = static_cast<int>(T*.025);
    std::vector<long > v(&times[n][0], (&times[n][0]) + T);
    std::sort(v.begin(), v.end());
    std::vector<long> d(v.begin()+exclude, v.end()-exclude);

    sum = std::accumulate(d.begin(), d.end(), 0);
    ct =  d.size();

    min = d[0];
    max = d.back();

    mean = (double)sum/(double)ct;
    for( auto dd : d)
        calc+= (dd - mean)*(dd- mean);
#if 0
  min = max = sum = times[n][0];
  
  for (i = 1; i < T; i++) {
    long t = times[n][i];
    if (t < min) { min = t; minIdx = i;}
    if (t > max) { max = t; maxIdx = i;}
    sum += t;
  }

  /* throw away lowest and highest. */
  sum = sum - min - max;
  ct = T - 2;

  mean = sum;
  mean = mean / ct;
  calc = 0;
  for (i = 0; i < T; i++) {
    if (i == minIdx || i == maxIdx) continue;
    calc += (times[n][i] - mean)*(times[n][i] - mean);
  }
#endif

  sprintf(buf, "%ld\t%f\t%ld\t%ld\t%f\t%d",
	  n, mean, min, max, sqrt(calc/(ct-1)), ct);
  return buf;
}

void buildTable(long** times, int R, int C) {
  int i;
  for (i = 0; i < R; i++) {
    printf ("%s\n", buildRow (i, times, C));
  }
  printf ("\n");
}

void port_gettime (struct timespec *ts) {
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, ts); 
}

/**
 * Compute the difference between two time values in usecs. 
 */
long diffNanoTimer (struct timespec *before, struct timespec *after) {
  long ds = after->tv_sec - before->tv_sec;
  long nds = after->tv_nsec - before->tv_nsec;

  /* if secs are same, then return simple delta */
  if (ds == 0) {
    return nds;
  }

  /* ok, so we are turned over. Something like: */
  /* before: 1179256745 744597                  */
  /* after:  1179256746 73514                   */

  /* if we have 'turned over' then account properly */
  if (nds < 0) {
    ds = ds - 1;
    nds += 1000000000;
  }

  return 1000000000*ds + nds;
}


#ifdef COUNT
void printComparisons() {
  printf ("There were a total of %ld comparisons\n", COMP_COUNT);
  printf ("There were a total of %ld Nil comparisons\n", NIL_COUNT);
  printf ("Resulting in %ld element swaps\n", SWAP_COUNT);
}
#endif /* COUNT */


/**
 * Standard reporting system.
 * \param usecs   absolute time amount.
 */
void report(long usecs) {
  printDiffTimer(usecs);

#ifdef COUNT
  printComparisons();
#endif /* COUNT */
}

/** No special usage. */
void reportUsage() {

}

