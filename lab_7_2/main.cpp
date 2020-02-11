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

    qDebug() << chars.contains( QRegExp("[a-z,A-Z]") );

    if ( chars.contains( "^\-+" ) )
        qCritical() << "It's a negative number!";

    conf_file.close();

    QList<int> number;

//    foreach ( const auto& ch, chars )
//        number.append( ch.toInt() );

    qDebug() << "Chars: " << chars;
    qDebug() << "Number: " << number;

    //-----------------------------------------------------------------------------------

    return qapp.exec();
}
//=======================================================================================
