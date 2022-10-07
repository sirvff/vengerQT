#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile styleSheetFile(":/new/prefix1/Adaptic.qss");
                    styleSheetFile.open(QFile::ReadOnly);
                    QString styleSheet = QLatin1String(styleSheetFile.readAll());
                    a.setStyleSheet(styleSheet);
    MainWindow w;
    w.showMaximized();

    return a.exec();
}
