#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <QObject>

#include "mqtt/async_client.h"

#define CLIENT_ID "fit-mqtt-explorer"

/**
 * @brief The MQTTManager class wraps around mqtt:async_client in a Qt
 * compatible manner
 */
class MQTTManager : public QObject
{
    Q_OBJECT
public:
    bool connected; //<whether the client is currently connected

    explicit MQTTManager(QObject *parent = nullptr);

public slots:
    /**
     * @brief connect connects to an MQTT server
     * @details Currently supports connection to unsaved adresses
     * from newConnection window
     * @param fullConnectionAdress string with full server adress with port
     */
    void connect(QString fullConnectionAdress);

signals:
    /**
     * @brief connectedChanged is emitted when the connection status of the
     * client changes
     * @param connected whether client is connected or not
     */
    void connectedChanged(bool connected);
    /**
     * @brief messageReceived is emitted when a message arrives from the server
     * @param msg See mqtt::message
     */
    void messageReceived(const mqtt::const_message_ptr msg);
    /**
     * @brief messageDelivered is emitted when delivery for a message has been
     * completed, and all acknowledgments have been received
     * @param tok See mqtt::delivery_token
     */
    void messageDelivered(mqtt::delivery_token_ptr tok);
    /**
     * @brief operationFailed is emitted when an action fails
     * @param tok See mqtt::token
     */
    void operationFailed(const mqtt::token& tok);
    /**
     * @brief operationSucceeded is emitted when an action has completed
     * successfully
     * @param tok See mqtt::token
     */
    void operationSucceeded(const mqtt::token& tok);

private:
    mqtt::async_client_ptr client;
    mqtt::connect_options_ptr options;
    mqtt::callback_ptr cb;

};

#endif // MQTTMANAGER_H
