#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define NUMT	         1	// number of threads to use -- do once for 1 and once for 4
#define MULTINUMT	         4	// number of threads to use -- do once for 1 and once for 4

#define SIZE       	16384	// array size -- you get to decide
#define NUMTRIES        20	// how many times to run the timing to get reliable timing data

float A[SIZE];
float B[SIZE];
float C[SIZE];

double peakPerformance (int num)
{
     // inialize the arrays:
	for( int i = 0; i < SIZE; i++ )
	{
		A[i] = 1.;
		B[i] = 2.;
	}

        omp_set_num_threads( num );

        double maxMegaMults = 0.;

        for( int t = 0; t < NUMTRIES; t++ )
        {
                double time0 = omp_get_wtime( );

                #pragma omp parallel for
                for( int i = 0; i < SIZE; i++ )
                {
                        C[i] = A[i] * B[i];
                }

                double time1 = omp_get_wtime( );
                double megaMults = (double)SIZE/(time1-time0)/1000000.;
                if( megaMults > maxMegaMults )
                        maxMegaMults = megaMults;
        }
        fprintf( stderr, "For %d thread(s), Peak Performance = %8.2lf MegaMults/Sec\n", num, maxMegaMults );

        return maxMegaMults;
}

int
main( )
{
#ifdef   _OPENMP
	fprintf( stderr, "OpenMP version %d is supported here\n", _OPENMP );
#else
	fprintf( stderr, "OpenMP is not supported here\n" );
	exit( 0 );
#endif

	double one_thread = peakPerformance(NUMT);
        double four_thread = peakPerformance(MULTINUMT);
        double S = four_thread / one_thread;
        float Fp = (4./3.)*( 1. - (1./S) );
        
        fprintf( stderr, "The S is %8.2lf\n", S);
        fprintf( stderr, "The Fp is %8.2lf\n", Fp);

	// note: %lf stands for "long float", which is how printf prints a "double"
	//        %d stands for "decimal integer", not "double"

	// Speedup = (Peak performance for 4 threads) / (Peak performance for 1 thread)

        return 0;
}

