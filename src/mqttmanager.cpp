/**
 * @file mqttmanager.cpp
 * @author Ondřej Míchal <xmicha80>
 */

#include <iostream>
#include <QDebug>

#include "mqtt/message.h"
#include "mqttmanager.h"

/**
 * @brief Representation of MQTT callback used to handle async events of an
 * async MQTT client
 */
class MQTTCallback :
    public virtual mqtt::callback,
    public virtual mqtt::iaction_listener
{
public:
    explicit MQTTCallback(MQTTManager* mqtt_manager) :
        mqtt_manager(mqtt_manager)
    {

    }

    void on_success(const mqtt::token& tok) override
    {
        emit this->mqtt_manager->onOperationSucceeded(tok);
    }

    void on_failure(const mqtt::token& tok) override
    {
        emit this->mqtt_manager->onOperationFailed(tok);
    }

    void connected(const std::string& cause) override
    {
        this->mqtt_manager->connected = true;
        emit this->mqtt_manager->onConnected();
    }

    void connection_lost(const std::string& cause) override
    {
        this->mqtt_manager->connected = false;
        emit this->mqtt_manager->onDisconnected();
    }

    void message_arrived(mqtt::const_message_ptr msg) override
    {
        emit this->mqtt_manager->onMessageReceived(msg);
    }

    void delivery_complete(mqtt::delivery_token_ptr tok) override
    {
        emit this->mqtt_manager->onMessageDelivered(tok);
    }

private:
    MQTTManager* mqtt_manager;
};

MQTTManager::MQTTManager(QObject *parent) :
    QObject(parent),
    client(nullptr),
    options(nullptr),
    cb(nullptr)
{
    this->connected = false;

    QObject::connect(this, &MQTTManager::onConnected, this, &MQTTManager::setupDefaultSubscriptions);
}

const QString MQTTManager::getServerName()
{
    if (this->client == nullptr) {
        return QString();
    }

    return QString::fromStdString(this->client->get_server_uri());
}

void MQTTManager::connect(QString fullConnectionAdress)
{
    if (this->client != nullptr) {
        if (this->client->is_connected()) {
            return;
        }
    }

    this->client = std::make_shared<mqtt::async_client>(fullConnectionAdress.toStdString(), CLIENT_ID);

    this->options = std::make_shared<mqtt::connect_options>();
    this->options->set_clean_session(true);
    this->options->set_automatic_reconnect(true);

    auto cb = std::make_shared<MQTTCallback>(this);
    this->cb = cb;
    client->set_callback(*this->cb);

    this->client->start_consuming();

    try {
        auto tok = this->client->connect(*this->options, nullptr, *cb);
    } catch (mqtt::exception err) {
        return;
    }
}

void MQTTManager::send(mqtt::const_message_ptr msg)
{
    if (this->client == nullptr) {
        return;
    }

    if (!this->client->is_connected()) {
        return;
    }

    this->client->publish(msg);
}

void MQTTManager::setupDefaultSubscriptions()
{
    if (this->client->is_connected()) {
        this->client->subscribe("#", 0);
    }
}
