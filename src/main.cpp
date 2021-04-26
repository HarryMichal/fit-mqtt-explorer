#include <QApplication>

#include "mainwindow.h"

Q_DECLARE_METATYPE(mqtt::const_message_ptr)

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;

    qRegisterMetaType<mqtt::const_message_ptr>("mqtt::const_message_ptr");

    app.setApplicationName("fit-mqtt-client");
    app.setApplicationDisplayName("MQTT Client");
    app.setApplicationVersion("1.0");

    window.show();

    return app.exec();
}
