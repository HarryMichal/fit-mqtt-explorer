#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <QObject>
#include <QString>

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

const QString getServerName();

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
     * @brief onConnected is emitted when the client connects to a server
     */
    void onConnected();
    /**
     * @brief onDisconnected is emitted when the client disconnects from a
     * server
     */
    void onDisconnected();
    /**
     * @brief onMessageReceived is emitted when a message arrives from the server
     * @param msg See mqtt::message
     */
    void onMessageReceived(const mqtt::const_message_ptr msg);
    /**
     * @brief onMessageDelivered is emitted when delivery for a message has been
     * completed, and all acknowledgments have been received
     * @param tok See mqtt::delivery_token
     */
    void onMessageDelivered(mqtt::delivery_token_ptr tok);
    /**
     * @brief onOperationFailed is emitted when an action fails
     * @param tok See mqtt::token
     */
    void onOperationFailed(const mqtt::token& tok);
    /**
     * @brief onOperationSucceeded is emitted when an action has completed
     * successfully
     * @param tok See mqtt::token
     */
    void onOperationSucceeded(const mqtt::token& tok);

private:
    mqtt::async_client_ptr client;
    mqtt::connect_options_ptr options;
    mqtt::callback_ptr cb;
};

#endif // MQTTMANAGER_H
