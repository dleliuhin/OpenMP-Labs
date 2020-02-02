/*
 * 12. Охранное агентство разработало новую систему управления элек-
 * тронными замками. Для открытия двери клиент обязан произнести произ-
 * вольную фразу из 25 слов. В этой фразе должно встречаться заранее огово-
 * ренное слово, причем только один раз. Требуется создать OpenMP-
 * приложение, управляющее замком. Потоки должны осуществлять сравнение
 * параллельно по 5 слов.
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
struct Params
{
    QString key;
    int words_per_thread;
};
//=======================================================================================

//=======================================================================================
int main( int argc, char **argv )
{
    QCoreApplication qapp( argc, argv );

    auto key_count = 0;

    //-----------------------------------------------------------------------------------

    auto qargs = qapp.arguments();

    if ( !qargs.contains( "-c" ) )
        qWarning() << "The arguments doesn't contains flag -c!";

    auto cfg_path = qargs.at( qargs.indexOf( "-c" ) + 1 );

    if ( !qargs.contains( "-p" ) )
        qWarning() << "The arguments doesn't contains flag -p!";

    auto param_path = qargs.at( qargs.indexOf( "-p" ) + 1 );

    //-----------------------------------------------------------------------------------

    auto settings = new QSettings( param_path, QSettings::IniFormat, &qapp );

    if ( settings->status() != QSettings::NoError )
        qCritical() << "No config specified for path: ";

    Params params;
    {
        settings->beginGroup( "params" );
        params.key = settings->value( "key", "" ).toString();
        params.words_per_thread = settings->value( "words_per_thread", 0 ).toInt();
        settings->endGroup();
    }

    //-----------------------------------------------------------------------------------

    QFile conf_file( cfg_path, &qapp );

    if ( !conf_file.open( QIODevice::ReadOnly | QIODevice::Text ) )
        qCritical() << "Error cannot open file:" << &conf_file;

    QTextStream qts( &conf_file );
    qts.setCodec( "UTF-8" );

    auto words = qts.readAll().split( " " );
    words.last().chop(1);

    conf_file.close();

    qDebug() << "Key word: " << params.key;
    qDebug() << "Code phrase: " << words;

    //-----------------------------------------------------------------------------------

    omp_set_num_threads( qCeil( words.size() / qreal( params.words_per_thread ) ) );

#pragma omp parallel reduction( + : key_count )
    {
        auto thr_num = omp_get_thread_num();

        auto subwords = words.mid( thr_num * params.words_per_thread,
                                   params.words_per_thread );

        for ( const auto& word: subwords )
            if ( word.compare( params.key ) == 0 )
                key_count++;
    }

    //-----------------------------------------------------------------------------------

    qDebug() << key_count;

    qDebug() << ( key_count == 1 ? "Unlocked" : "Locked" );

    return qapp.exec();
}
//=======================================================================================
