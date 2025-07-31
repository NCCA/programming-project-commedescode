#include "MainWindow.h"
#include <QApplication>


int main(int _argc, char *_argv[])
{
    QSurfaceFormat format;
    format.setMajorVersion(4);
    format.setMinorVersion(6);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication application(_argc, _argv);
    MainWindow window;
    window.show();

    return application.exec();
}
