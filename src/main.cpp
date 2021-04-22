#include <QApplication>
#include <QCommandLineParser>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    app.setApplicationName("fit-mqtt-client");
    app.setApplicationDisplayName("MQTT Client");
    app.setApplicationVersion("1.0");

    MainWindow window;
    window.show();

    return app.exec();
}
