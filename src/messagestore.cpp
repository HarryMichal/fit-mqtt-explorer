#include "messagestore.h"

MessageStore::MessageStore(QObject *parent) :
    QObject(parent),
    ticker(new QTimer(this)),
    message_capacity(50),
    messages(QHash<QString, QList<QString>>()),
    new_messages(QHash<QString, QList<QString>>())
{
    connect(this->ticker, &QTimer::timeout, this, &MessageStore::handleTick);
    this->ticker->start(200);
}

void MessageStore::setMessageCap(int cap)
{
    this->message_capacity = cap;
}

int MessageStore::getMessageCap()
{
    return this->message_capacity;
}

const QHash<QString, QList<QString>> MessageStore::getAllMessages()
{
    return this->messages;
}

const QList<QString> MessageStore::getTopicMessages(const QString topic)
{
    if (!this->messages.contains(topic)) {
        return QList<QString>();
    }

    return this->messages[topic];
}

void MessageStore::addMessage(const mqtt::const_message_ptr msg)
{
    // This works for both scenarios: topic is known and topic is unknown.
    // Using the []operator to get an item causes the creation of a default
    // item than can be used to add the new message payload.
    auto qtopic = QString::fromStdString(msg->get_topic());
    QList<QString> msg_list = this->new_messages[qtopic];

    msg_list.append(QString::fromStdString(msg->get_payload_str()));

    this->new_messages.insert(qtopic, msg_list);
}

void MessageStore::handleTick()
{
    if (this->new_messages.empty()) {
        return;
    }

    emit this->newMessages(this->new_messages);
    this->new_messages.clear();
}
