#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFileDialog>
#include <QMessageBox>
#include "kmldialog.h"
#include <QtQml>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
   qmlRegisterType<KMLDialog>("demo.kmlDialog", 1, 0, "KMLDialogQml");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
