#include "../include/util.h"



void ut_msleep (unsigned long ms)
{
	usleep(1000*ms);
}



void ut_current_utc_time (struct timespec *ts)
{
    #ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
        clock_serv_t cclock;
        mach_timespec_t mts;
        host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
        clock_get_time(cclock, &mts);
        mach_port_deallocate(mach_task_self(), cclock);
        ts->tv_sec = mts.tv_sec;
        ts->tv_nsec = mts.tv_nsec;
    #else
        clock_gettime(CLOCK_REALTIME, ts);
    #endif
}



unsigned long ut_time_in_usec ()
{
    unsigned long   us; // Microseconds
    time_t          s;  // Seconds
    struct timespec spec;

    ut_current_utc_time(&spec);
    s  = spec.tv_sec;
    us = round(spec.tv_nsec / 1.0e3); // Convert nanoseconds to microseconds

    unsigned long x = (long)(intmax_t)s;

    return x*1.0e6 + us;
}



unsigned long ut_time_in_msec ()
{
    unsigned long   ms; // Milliseconds
    time_t          s;  // Seconds
    struct timespec spec;

    ut_current_utc_time(&spec);
    s  = spec.tv_sec;
    ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds

    unsigned long x = (long)(intmax_t)s;

    return x*1000 + ms;
}



double ut_interval_in_msec (unsigned long t0, unsigned long t1)
{
    return (double)(t1-t0)/1000.0;
}



ut_sysinfo_t* ut_get_sysinfo ()
{
    int status, line, i;
    static char flag = 0;
    static struct sysinfo info;

    if (TRUE || !flag) // This really must be called every time...
    {
        status = sysinfo(&info);
        line = __LINE__ - 1;

        if (status != 0)
        {
            ut_errmsg (
                "Call to sysinfo() returned an error.",
                __FILE__, line, 1
            );
        }
    }

    ut_sysinfo_t* newinfo = (ut_sysinfo_t*) malloc (sizeof(ut_sysinfo_t));
    newinfo->uptime    = info.uptime;
    newinfo->loads[0]  = info.loads[0];
    newinfo->loads[1]  = info.loads[1];
    newinfo->loads[2]  = info.loads[2];
    newinfo->totalram  = info.totalram;
    newinfo->freeram   = info.freeram;
    newinfo->sharedram = info.sharedram;
    newinfo->bufferram = info.bufferram;
    newinfo->totalswap = info.totalswap;
    newinfo->freeswap  = info.freeswap;
    newinfo->procs     = info.procs;
    newinfo->totalhigh = info.totalhigh;
    newinfo->freehigh  = info.freehigh;
    newinfo->mem_unit  = info.mem_unit;
    for (i = 0; i < 20-2*sizeof(long)-sizeof(int); i++)
        newinfo->_f[i] = info._f[i];

    return newinfo;
}



void ut_errmsg (const char* msg, const char* file, int line, int stop)
{
    fprintf(stderr, "Error at %s:%d. ", file, line);
    fprintf(stderr, "%s\n", msg);

    if (stop)
    {
        exit(1);
    }
}



double ut_gaussian_noise (double mu, double sigma)
{
	static int generateGaussianNoise_seeded = 0;
	if (generateGaussianNoise_seeded == 0) 
    {
        srand(time(NULL));
        generateGaussianNoise_seeded = 1;
	}

	static const double epsilon = DBL_MIN;//std::numeric_limits<double>::min();
	static const double two_pi = 2.0*3.14159265358979323846;

	//thread_local double z0, z1;
	double z0, z1;
	//thread_local bool generate;
	//bool generate;
	char generate;
	generate = !generate;

	if (!generate)
        return z1 * sigma + mu;

	double u1, u2;
	do
    {
        u1 = rand() * (1.0 / RAND_MAX);
        u2 = rand() * (1.0 / RAND_MAX);
    }
	while ( u1 <= epsilon );

	z0 = sqrt(-2.0 * log(u1)) * cos(two_pi * u2);
	z1 = sqrt(-2.0 * log(u1)) * sin(two_pi * u2);
	return z0 * sigma + mu;
}



double ut_gaussian_rand()
{
	return ut_gaussian_noise(0,1);
}
