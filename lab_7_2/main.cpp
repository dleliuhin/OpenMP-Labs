/*
 * 12. Определить, делится ли целое число А, содержащее от 1 до 1000 значащих цифр,
 * на 2, 3, 4, 5, 6, 7, 8, 9, 10. Входные данные: целое положительное число А, записанное
 * в файле. Количество потоков зависит числа А, и не является параметром задачи.
 */
//=======================================================================================

#include <QStringList>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QtMath>
#include <QSettings>

#include <omp.h>

//=======================================================================================
int main( int argc, char **argv )
{
    QCoreApplication qapp( argc, argv );

    //-----------------------------------------------------------------------------------

    static QMap<int, bool> decision;

    auto qargs = qapp.arguments();

    if ( !qargs.contains( "-c" ) )
        qWarning() << "The arguments doesn't contain flag -c!";

    auto cfg_path = qargs.at( qargs.indexOf( "-c" ) + 1 );

    //-----------------------------------------------------------------------------------

    QFile conf_file( cfg_path, &qapp );

    if ( !conf_file.open( QIODevice::ReadOnly | QIODevice::Text ) )
        qCritical() << "Error cannot open file:" << &conf_file;

    QTextStream qts( &conf_file );
    qts.setCodec( "UTF-8" );

    auto chars = qts.readAll();
    chars.chop(1);

    conf_file.close();

    Q_ASSERT( !chars.contains( QRegExp("[a-z,A-Z,-/+]") ) );
    Q_ASSERT( !chars.isEmpty() );
    Q_ASSERT( !( chars.size() > 1000 ) );

    QList<int> numbers;

    for ( const auto& ch: chars.toUtf8() )
        numbers.append( ch - '0' );

    qDebug() << "Chars: " << chars;
    qDebug() << "Number: " << numbers;

    //-----------------------------------------------------------------------------------

#pragma omp parallel sections num_threads(3)
    {
        //--------------------------------------------------------------------------------

        // Check divisibility by 2

#pragma omp section
        {
#pragma omp parallel if ( numbers.last() % 2 == 0 )
            if ( omp_in_parallel() )
            {
                decision[2] = true;
            }
            else
            {
                decision[2] = false;
            }
        }

        //--------------------------------------------------------------------------------

        // Check divisibility by 5

#pragma omp section
        {
#pragma omp parallel if ( numbers.last() % 5 == 0 )
            if ( omp_in_parallel() )
            {
                decision[5] = true;
            }
            else
            {
                decision[5] = false;
            }
        }

        //--------------------------------------------------------------------------------

        // Check divisibility by 10

#pragma omp section
        {
#pragma omp parallel if ( numbers.last() == 0 )
            if ( omp_in_parallel() )
            {
#pragma omp single
                decision[10] = true;
            }
            else
            {
                decision[10] = false;
            }
        }
    }

    //-----------------------------------------------------------------------------------

    int sum = 0;

    omp_set_num_threads( numbers.size() );
#pragma omp parallel
    {
#pragma omp for reduction( +: sum ) // Можно и #pragma omp atomic
        for ( int i = 0; i < numbers.size(); i++ )
            sum += numbers.at(i);
    }

    //-----------------------------------------------------------------------------------

    // Check divisibility by 3

#pragma omp parallel if ( sum % 3 == 0 )
    if ( omp_in_parallel() )
    {
#pragma omp single
        decision[3] = true;
    }
    else
    {
        decision[3] = false;
    }

    //-----------------------------------------------------------------------------------

    // Check divisibility by 9

#pragma omp parallel if ( sum % 9 == 0 )
    if ( omp_in_parallel() )
    {
#pragma omp single
        decision[9] = true;
    }
    else
    {
        decision[9] = false;
    }

    //-----------------------------------------------------------------------------------

    // Check divisibility by 4

    sum = 0;


    //-----------------------------------------------------------------------------------

    for ( const auto& key: decision.keys() )
        if ( decision.value( key ) )
            qDebug() << "The number can divised by " << key;
        else
            qDebug() << "The number cannot divised by " << key;

    return 0;
}
//=======================================================================================
