
/****************VALUE OF PI****************/
#include<stdio.h>
#include<math.h>
#include <omp.h>
#include <sys/time.h>
/**Function that generates a random number*/
int rand(int r0, int a, int m, int c){
    double r1=(a*r0+c)%m;
    return r1;
}
/**Function that generates random numbers given a seed, and stores them in an array that is passed as an argument*/
void randNos(int r0, int a, int m, int c, int n, int x[n]){
    double r1=rand(r0,a,m,c);
    int i;
    for(i=0;i<n;i++){
        x[i]=r1;
        r1=rand(r1,a,m,c);
    }
}
/**Function that generates random numbers in a given range: [min,max], given a seed r0,
 * and stores them in an array that is passed as an argument.*/
void randNosRange(int n, double r[n], double x[n], double min, double max){
    int i;
    double r1;
    for(i=0;i<n;i++){
        r1=min+(max-min)*r[i];
        x[i]=r1;
    }
}

long double Calculate_Pi_Sequential(long n)
{
    int min = -1, max = 1, a = 1093, m = 86436, c = 18257;
    double x0 = 43, y0 = 21; //x-coordinates, y-coordinates
    int i, j, k, l, xtemp[n], ytemp[n];
    randNos(x0, a, m, c, n, xtemp); //this would store random nos. from 0 to m-1 in xtemp for a given seed
    randNos(y0, a, m, c, n, ytemp);  //this would store random nos. from 0 to m-1 in ytemp for a given seed
    double xtemp1[n], ytemp1[n];
    //Renormalize the randomnumbers so that their values are from within [0,1]
    for (i = 0; i < n; i++) {
        xtemp1[i] = (double) xtemp[i] / (m - 1);
        ytemp1[i] = (double) ytemp[i] / (m - 1);
    }
    double x[n], y[n];
    randNosRange(n, xtemp1, x, -1, 1);
    randNosRange(n, ytemp1, y, -1, 1);
    int Naccept = 0;
    for (i = 0; i < n; i++) {
        double s = pow(x[i], 2) + pow(y[i], 2);
        if (s <= 1.0) {
            Naccept++;
        }
    }
    double pi;
    pi = 4 * (double) Naccept / n;
    return pi;
}

long double Calculate_Pi_Parallel(long n)
{
    //Parallel mutiple threads
#pragma omp parallel num_threads(omp_get_max_threads())
    {

        int min = -1, max = 1, a = 1093, m = 86436, c = 18257;
        double x0 = 43, y0 = 21; //x-coordinates, y-coordinates
        int i, j, k, l, xtemp[n], ytemp[n];
        randNos(x0, a, m, c, n, xtemp); //this would store random nos. from 0 to m-1 in xtemp for a given seed
        randNos(y0, a, m, c, n, ytemp);  //this would store random nos. from 0 to m-1 in ytemp for a given seed
        double xtemp1[n], ytemp1[n];
        //Renormalize the randomnumbers so that their values are from within [0,1]
        for (i = 0; i < n; i++) {
            xtemp1[i] = (double) xtemp[i] / (m - 1);
            ytemp1[i] = (double) ytemp[i] / (m - 1);
        }
        double x[n], y[n];
        randNosRange(n, xtemp1, x, -1, 1);
        randNosRange(n, ytemp1, y, -1, 1);
        int Naccept = 0;
        for (i = 0; i < n; i++) {
            double s = pow(x[i], 2) + pow(y[i], 2);
            if (s <= 1.0) {
                Naccept++;
            }
        }
        double pi;
        pi = 4 * (double) Naccept / n;
        return 0;
    }

}

main(){
    struct timeval start, end;
    long  n;
    n = 10000000;
    gettimeofday(&start, NULL);
    printf("Timing sequentiona1...\n");
    long double sequential_pi = Calculate_Pi_Sequential(n);
    gettimeofday(&end, NULL);
    printf("Took %.6f seconds\n", end.tv_sec - start.tv_sec + (double) (end.tv_usec - start.tv_usec) / 1000000L);

    printf("Timing parallel...\n");
    gettimeofday(&start, NULL);
    long double parallel_pi = Calculate_Pi_Parallel(n);
    gettimeofday(&end, NULL);
    printf("Took %.6f seconds\n", end.tv_sec - start.tv_sec + (double) (end.tv_usec - start.tv_usec) / 1000000L);
     // This will print the result to 10 decimal places
    printf("p = %.10Lf (sequential)\n", sequential_pi);
    printf("p = %.10Lf (parallel)", parallel_pi);


    return 0;

}