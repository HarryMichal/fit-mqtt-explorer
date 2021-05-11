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
 * @brief Represents the currently visible page
 */
enum ModeState {
    Dashboard,
    Explorer,
    Simulator
};

/**
 * @brief Represents the main window of the application
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /**
     * @brief Sets the dashboard page as the primary one
     */
    void setDashboardPage();
    /**
     * @brief Sets the explorer page as the primary one
     */
    void setExplorerPage();
    /**
     * @brief Sets the simulator page as the primary one
     */
    void setSimulatorPage();
    /**
     * @brief Spawns connection window
     */
    void OpenConnectionWindow();

private:
    Ui::MainWindow *ui;
    QThread worker_thread; //< Thread for running MQTTManager
    MessageStore *msg_store; //< Store of messages from a MQTT broker
    MQTTManager *mqtt_manager; //< Manager of connection to a MQTT broker
    DashboardPage *dashboard; //< Dashboard page
    ExplorerPage *explorer; //< Explorer page
    SimulatorPage *simulator; //< Traffic simulator
    NewConnection *new_connection_window;

    ModeState current_mode; //< Currently selected page
    QActionGroup *mode_selector; //< Group of actions for selecting pages

    QLabel connection_status; //<Permanent widget in StatusBar

    /**
     * @brief Sets up status bar
     */
    void setupStatusBar();
    /**
     * @brief Sets up global actions
     */
    void setupActions();
    /**
     * @brief Updates the content of status bar
     */
    void updateStatusBar();
    /**
     * @brief Handles the result of connection window. Initiates a new
     * connection via MQTTManager
     */
    void createNewConnection();

private slots:
    /**
     * @brief Handles operations needed to be done immediately after conneting
     * to a broker
     */
    void clientConnected();
    /**
     * @brief Opens a file dialog asking the user for a folder where a snapshot
     * of the state of explorer should be saved
     */
    void createSnapshot();
    /**
     * @brief Handles changing of selected message in explorer. Gets the needed
     * message from message store and sends it to explorer back
     *
     * @param currentRow selected row
     */
    void explorerChangeSelectedMessage(const int currentRow);
    /**
     * @brief Handles changing of selected topic in explorer. Gets messages
     * tied to the topic from message store and send them to explorer back.
     */
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
