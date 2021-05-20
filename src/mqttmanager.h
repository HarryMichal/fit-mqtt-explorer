/**
 * @file mqttmanager.h
 * @author Ondřej Míchal <xmicha80>
 */

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
    bool connected; //< whether the client is currently connected

    explicit MQTTManager(QObject *parent = nullptr);

    /**
     * @brief Returns the name of the server the client is currently connected
     * to
     */
    const QString getServerName();

public slots:
    /**
     * @brief connect connects to an MQTT server
     * @details Currently supports connection to unsaved adresses
     * from newConnection window
     * @param fullConnectionAdress string with full server adress with port
     */
    void connect(QString fullConnectionAdress, QString client_id);
    /**
     * @brief send() sends a message to an MQTT server
     * @details The assembly of a message is to be done by other components.
     * Not this method.
     *
     * @param msg an already prepared message to be sent
     */
    void send(mqtt::const_message_ptr msg);

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

    void setupDefaultSubscriptions();
};

#endif // MQTTMANAGER_H
