#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QActionGroup>
#include <QLabel>
#include <QMainWindow>
#include <QThread>

#include "mqttmanager.h"
#include "newconnection.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @brief The ModeState enum represents visible page
 */
enum ModeState {
    Dashboard,
    Explorer
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void SetDashboardPage();
    void SetExplorerPage();
    void OpenConnectionWindow();


private:
    Ui::MainWindow *ui;
    QThread worker_thread; //<Thread for running MQTTManager
    MQTTManager *mqtt_manager; //<Manager of connection to a MQTT broker
    NewConnection *new_connection_window;

    ModeState currentMode;
    QActionGroup *modeSelector;

    QLabel connection_status; //<Permanent widget in StatusBar

    void setupStatusBar();
    void setupActions();
    void updateStatusBar();

private slots:
    /**
     * @brief messageReceived is emitted when a message arrives from the server.
     * @param msg See mqtt::message
     */
    void messageReceived(const mqtt::const_message_ptr msg);
};

#endif // MAINWINDOW_H
