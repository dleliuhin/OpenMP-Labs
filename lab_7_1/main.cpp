/*
 * 12. openmp_itc_12.c
 * ...
 * double x[N],y[N];
 * x[0] = 0;
 * # pragma omp parallel for nowait
 * for (i=1; i<N; i++)
 * {
 *      x[i] = x[i-1]*log(i);
 *      y[i] = x[i] /y[i-1];
 * }
 * y[0]=x[N-1];
 * ...
 */
//=======================================================================================

#include <QCoreApplication>
#include <QDebug>
#include <QtMath>

#include <omp.h>

static constexpr auto N = 5;

//=======================================================================================
int main( int argc, char **argv )
{
    QCoreApplication qapp( argc, argv );

    double x[N], y[N];
    x[0] = 1;

#pragma omp parallel for ordered
    for ( auto i = 1; i < N; i++ )
    {
#pragma omp ordered
        {
            x[i] = x[i-1] * log(i);

            if ( ( i > 1 ) && !( y[i-1] < std::numeric_limits<double>::epsilon() ) )
                y[i] = x[i] / y[i-1];
            else
                y[i] = 0;
        }
    }

    y[0] = x[N-1];

    return qapp.exec();
}
//=======================================================================================
