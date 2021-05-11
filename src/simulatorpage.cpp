/**
 * @file simulatorpage.cpp
 * @author Ondřej Míchal <xmicha80>
 */

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include "simulatorpage.h"
#include "ui_simulatorpage.h"

MockDevice::MockDevice(QString topic, QString type, int period, QList<QVariant> payloads, QObject *parent) :
    QObject(parent),
    topic(topic),
    type(type),
    period(period),
    payloads(payloads),
    payload_iterator(QMutableListIterator<QVariant>(payloads))
{
    this->heartbeat = new QTimer(this);
    connect(this->heartbeat, &QTimer::timeout, this, &MockDevice::handleHeartbeat);

    this->payload_iterator = QMutableListIterator<QVariant>(this->payloads);
}

MockDevice::~MockDevice()
{
    delete this->heartbeat;
}

void MockDevice::start()
{
    this->heartbeat->start(this->period);
}

void MockDevice::stop()
{
    this->heartbeat->stop();
}

void MockDevice::handleHeartbeat()
{
    if (!payload_iterator.hasNext()) {
        this->payload_iterator.toFront();
    }
    this->payload_iterator.next();

    mqtt::string topic = this->topic.toStdString();
    mqtt::string payload = this->payload_iterator.value().toString().toStdString();

    emit this->onHeartbeat(topic, payload);
}

SimulatorPage::SimulatorPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimulatorPage),
    device_pool(new QList<MockDevice*>)
{
    ui->setupUi(this);
}

SimulatorPage::~SimulatorPage()
{
    delete this->ui;
    delete this->device_pool;
}

void SimulatorPage::setSimulationConfigFile(QString filename)
{
    QFile config_file = QFile(filename);

    if (!config_file.exists()) {
        return;
    }

    if (!config_file.open(QFile::ReadOnly)) {
        return;
    }

    auto data = config_file.readAll();

    this->config = QJsonDocument::fromJson(data);
    if (this->config.isNull()) {
        return;
    }

    this->configureSimulation();
}

void SimulatorPage::startSimulation()
{
    for (auto it = this->device_pool->begin(); it != this->device_pool->end(); it++) {
        (*it)->start();
    }
}

void SimulatorPage::stopSimulation()
{
    for (auto it = this->device_pool->begin(); it != this->device_pool->end(); it++) {
        (*it)->stop();
    }
}

void SimulatorPage::configureSimulation()
{
    QJsonValue value;

    // Clean the previous configuration
    this->stopSimulation();
    this->device_pool->clear();

    if (!this->config.isObject()) {
        return;
    }

    auto root = this->config.object();

    // The root object of the config file should have only 2 keys:
    //   - base topic name (string)
    //   - devices (array)
    if (root.length() != 2) {
        return;
    }

    value = root.value("topic");
    if (value.isUndefined() || !value.isString() ) {
        return;
    }
    this->base_topic = value.toString();

    value = root.value("devices");
    if (value.isUndefined() || !value.isArray()) {
        return;
    }
    auto device_list = value.toArray();

    // Iterate over all defined devices in the config and convert them into
    // MockDevice.
    for (auto it = device_list.begin(); it != device_list.end(); it++) {
        QJsonObject device;
        QString topic, type;
        int period;

        // Every device in the array has to be an object
        if (!it->isObject()) {
            return;
        }

        device = it->toObject();
       
        value = device.value("topic");
        if (value.isUndefined() || !value.isString()) {
            return;
        }
        topic = base_topic + "/" + value.toString();

        value = device.value("type");
        if (value.isUndefined() || !value.isString()) {
            return;
        }
        type = value.toString();

        value = device.value("period");
        if (value.isUndefined() || !value.isDouble()) {
            return;
        }
        period = value.toInt();

        value = device.value("payloads");
        if (value.isUndefined() || !value.isArray()) {
            return;
        }
        QList<QVariant> payloads = value.toArray().toVariantList();

        MockDevice *new_mock_device = new MockDevice(topic, type, period, payloads);
        connect(new_mock_device, &MockDevice::onHeartbeat, this, &SimulatorPage::onHeartbeat);
        this->device_pool->append(new_mock_device);
    }

    emit this->onReady();
}
