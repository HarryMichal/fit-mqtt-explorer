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

    void setDashboardPage();
    void setExplorerPage();
    void OpenConnectionWindow();

private:
    Ui::MainWindow *ui;
    QThread worker_thread; //<Thread for running MQTTManager
    MQTTManager *mqtt_manager; //<Manager of connection to a MQTT broker
    NewConnection *new_connection_window;

    ModeState current_mode;
    QActionGroup *mode_selector;

    QLabel connection_status; //<Permanent widget in StatusBar

    void setupStatusBar();
    void setupActions();
    void updateStatusBar();
};

#endif // MAINWINDOW_H
