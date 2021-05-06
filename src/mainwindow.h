#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QActionGroup>
#include <QLabel>
#include <QMainWindow>
#include <QThread>

#include "dashboardpage.h"
#include "explorerpage.h"
#include "messagestore.h"
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
    MessageStore *msg_store; //<Store of messages from a MQTT broker
    MQTTManager *mqtt_manager; //<Manager of connection to a MQTT broker
    DashboardPage *dashboard;
    ExplorerPage *explorer;
    NewConnection *new_connection_window;

    ModeState current_mode;
    QActionGroup *mode_selector;

    QLabel connection_status; //<Permanent widget in StatusBar

    void setupStatusBar();
    void setupActions();
    void updateStatusBar();
    void createNewConnection();

private slots:
    void clientConnected();
    void explorerChangeSelectedMessage(const int currentRow);
    void explorerChangeSelectedTopic(QString topic);
};

#endif // MAINWINDOW_H
