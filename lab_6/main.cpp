#include <omp.h>

#include <QCoreApplication>
#include <QDebug>
#include <QFile>

static constexpr auto key = "know";

int main( int argc, char **argv )
{
    QCoreApplication qapp( argc, argv );

    QVector<QString> words;

    QFile conf_file( "../lab_6/conf.txt" );

    if ( !conf_file.open( QIODevice::ReadOnly | QIODevice::Text ) )
        qCritical() << "Error cannot open file:" << &conf_file;

    QTextStream qts( &conf_file );
    qts.setCodec( "UTF-8" );

    while ( !qts.atEnd() )
        words.push_back( qts.readLine() );

    conf_file.close();

    qDebug() << words;

    #pragma omp parallel
    {
                auto count = omp_get_thread_num();
                auto ItsMe = omp_get_num_threads();

                qDebug( "Hello, OpenMP! I am %d of %d\n", count, ItsMe );
    }

    return qapp.exec();
}
