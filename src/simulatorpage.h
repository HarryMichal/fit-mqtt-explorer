/**
 * @file simulatorpage.h
 * @author Ondřej Míchal <xmicha80>
 */

#ifndef SIMULATORPAGE_H
#define SIMULATORPAGE_H

#include <QJsonDocument>
#include <QTimer>
#include <QWidget>

#include "mqtt/message.h"

namespace Ui {
class SimulatorPage;
}

/**
 * @brief Representation of a simulated device
 * @details Running MockDevice has a heartbeat that serves for sending messages
 * in a set interval.
 */
class MockDevice : public QObject
{
    Q_OBJECT

public:
    explicit MockDevice(QString topic, QString type, int period, QList<QVariant> payloads, QObject *parent = nullptr);
    ~MockDevice();

    /**
     * @brief Starts the devices heartbeat
     */
    void start();
    /**
     * @brief Stops the devices heartbeat
     */
    void stop();

signals:
    /**
     * @brief Is emitted when a new a message is to be sent
     */
    void onHeartbeat(mqtt::string topic, mqtt::string payload);

private slots:
    /**
     * @brief Reads the next payload and emits the onHeartbeat signal
     */
    void handleHeartbeat();

private:
    QTimer *heartbeat; //<Device's heartbeat

    QString topic; //<Topic where all messages are sent
    QString type; //<Type of the device
    int period; //<Period in which messages are sent
    QList<QVariant> payloads; //<List of all possible messages
    QMutableListIterator<QVariant> payload_iterator; //<Iterator over the list of all possible messages
};

/**
 * @brief Represents page for handling of traffic simulation
 */
class SimulatorPage : public QWidget
{
    Q_OBJECT

public:
    explicit SimulatorPage(QWidget *parent = nullptr);
    ~SimulatorPage();

    /**
     * @brief Reads the provided file, checks if it is a valid JSON and
     * initiates additional configuration
     */
    void setSimulationConfigFile(QString filename);
    /**
     * @brief Starts the simulation
     * @pre A simulation must be configured
     */
    void startSimulation();
    /**
     * @brief Stops the simulation
     * @pre A simulation must be configured
     */
    void stopSimulation();
signals:
    /**
     * @brief Is emitted when a new a message is to be sent
     */
    void onHeartbeat(mqtt::string topic, mqtt::string payload);
    /**
     * @brief Is emitted when a configuration has been successfully set
     */
    void onReady();

private:
    Ui::SimulatorPage *ui;
    
    QJsonDocument config; //<JSON configuration file of a simulation
    QList<MockDevice*> *device_pool; //<Pool of mocked devices

    QString base_topic; //<Root topic where all messages are sent

    /**
     * @brief Parses the read configuration file and tries to create mock
     * devices
     * @details The configuration file needs to conform to a specific format.
     * Check the main documentation file to learn more.
     */
    void configureSimulation();
};

#endif // SIMULATORPAGE_H
