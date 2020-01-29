/*
 * 12. Охранное агентство разработало новую систему управления элек-
 * тронными замками. Для открытия двери клиент обязан произнести произ-
 * вольную фразу из 25 слов. В этой фразе должно встречаться заранее огово-
 * ренное слово, причем только один раз. Требуется создать OpenMP-
 * приложение, управляющее замком. Потоки должны осуществлять сравнение
 * параллельно по 5 слов.
 */
//=======================================================================================

#include <omp.h>
#include <QStringList>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QtMath>

//=======================================================================================
int main( int argc, char **argv )
{
    QCoreApplication qapp( argc, argv );

    //-----------------------------------------------------------------------------------

    static constexpr auto key = "know";
    static auto key_count = 0;
    static auto words_per_thread = 5;

    //-----------------------------------------------------------------------------------

    QFile conf_file( "../lab_6/conf.txt" );

    if ( !conf_file.open( QIODevice::ReadOnly | QIODevice::Text ) )
        qCritical() << "Error cannot open file:" << &conf_file;

    QTextStream qts( &conf_file );
    qts.setCodec( "UTF-8" );

    auto words = qts.readAll().split( " " );

    conf_file.close();

    qDebug() << "Key word: " << key;
    qDebug() << words;

    omp_set_num_threads( qCeil( words.size() / qreal( words_per_thread ) ) );

#pragma omp parallel reduction( + : key_count )
    {
        auto count = omp_get_thread_num();
        auto ItsMe = omp_get_num_threads();

        key_count++;

        qDebug( "Hello, OpenMP! I am %d of %d\n", count, ItsMe );
    }

    qDebug() << key_count;

    return qapp.exec();
}
//=======================================================================================
