#ifdef __cplusplus
extern "C" {
#endif
#ifndef _UTIL_H_
#define _UTIL_H_

#include <math.h>
#include <time.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/sysinfo.h> // Linux

#ifdef __MACH__
	#include <mach/clock.h>
	#include <mach/mach.h>
#endif



#define FALSE 0
#define TRUE  !(FALSE)



struct ut_sysinfo 
{
    long uptime;             /* Seconds since boot */
    unsigned long loads[3];  /* 1, 5, and 15 minute load averages */
    unsigned long totalram;  /* Total usable main memory size */
    unsigned long freeram;   /* Available memory size */
    unsigned long sharedram; /* Amount of shared memory */
    unsigned long bufferram; /* Memory used by buffers */
    unsigned long totalswap; /* Total swap space size */
    unsigned long freeswap;  /* swap space still available */
    unsigned short procs;    /* Number of current processes */
    unsigned long totalhigh; /* Total high memory size */
    unsigned long freehigh;  /* Available high memory size */
    unsigned int  mem_unit;  /* Memory unit size in bytes */
    char _f[20-2*sizeof(long)-sizeof(int)]; /* Padding for libc5 */
};

typedef struct ut_sysinfo ut_sysinfo_t;



void          ut_msleep           (unsigned long ms);
void          ut_current_utc_time (struct timespec *ts);
unsigned long ut_time_in_us       ();
unsigned long ut_time_in_ms       ();
double        ut_interval_in_ms   (unsigned long t0, unsigned long t1);
ut_sysinfo_t* ut_get_sysinfo      ();

void ut_errmsg (const char* msg, const char* file, int line, int stop);

double ut_gaussian_noise (double mu, double sigma);
double ut_gaussian_rand  ();



#endif // _UTIL_H_
#ifdef __cplusplus
}
#endif
