#include <QApplication>
#include <QStyleFactory>
#include <QFontDatabase>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("Title Case Converter");
    app.setOrganizationName("berningo.com");
    app.setApplicationVersion("1.0.0");

#ifdef Q_OS_MACOS
    app.setStyle(QStyleFactory::create("macOS"));
#else
    app.setStyle(QStyleFactory::create("Fusion"));
#endif

    QFont defaultFont = app.font();
    defaultFont.setPointSize(12);
    app.setFont(defaultFont);

    MainWindow window;
    window.show();

    return app.exec();
}
