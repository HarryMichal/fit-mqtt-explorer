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
#include "simulatorpage.h"

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
    Explorer,
    Simulator
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setDashboardPage();
    void setExplorerPage();
    /**
     * @brief Sets the simulator page as the primary one
     */
    void setSimulatorPage();
    void OpenConnectionWindow();

private:
    Ui::MainWindow *ui;
    QThread worker_thread; //<Thread for running MQTTManager
    MessageStore *msg_store; //<Store of messages from a MQTT broker
    MQTTManager *mqtt_manager; //<Manager of connection to a MQTT broker
    DashboardPage *dashboard;
    ExplorerPage *explorer;
    SimulatorPage *simulator; //<Traffic simulator
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
    /*
     * @brief createSnapshot() opens a file dialog asking the user for a folder
     * where a snapshot of the state of explorer should be saved
     */
    void createSnapshot();
    void explorerChangeSelectedMessage(const int currentRow);
    void explorerChangeSelectedTopic(QString topic);
    /**
     * @brief sendFile() checks the existence of a file, reads it, assembles
     * a mesage and passes it to the MQTT manager
     *
     * @param topic topic where the message should be sent
     * @param file_name name of the file to be sent
     */
    void sendFile(mqtt::string topic, QString file_name);
    /**
     * @brief sendText() assembles a simple message and passes it to the MQTT
     * manager
     *
     * @param topic topic where the message should be sent
     * @param msg message to be sent
     */
    void sendText(mqtt::string topic, mqtt::string msg);
    /**
     * @brief setHistoryLimit() creates a dialog window for setting a new
     * message history limit and sets it to child components.
     */
    void setHistoryLimit();
    /**
     * @brief Spawns a file dialog window for choosing a configuration file of
     * a traffic simulation and sets the selected file in the simulator
     */
    void simulatorSetSimulationConfigFile();
    /**
     * @brief Toggles the simulation
     */
    void simulatorToggleSimulation(bool toggled);
};

#endif // MAINWINDOW_H
