#include <QDebug>
#include <QDir>

#include "messagestore.h"

Message::Message(mqtt::const_message_ptr msg, MessageType type, QObject *parent) :
    QObject(parent),
    msg(msg),
    msg_type(type),
    arrival_date(std::chrono::steady_clock::now())
{

}

std::string Message::getMessage()
{
    return this->msg->get_payload_str();
}

MessageStore::MessageStore(QObject *parent) :
    QObject(parent),
    ticker(new QTimer(this)),
    message_capacity(50),
    messages(QHash<QString, QList<Message*>>()),
    new_messages(QHash<QString, QList<Message*>>())
{
    connect(this->ticker, &QTimer::timeout, this, &MessageStore::handleTick);
    this->ticker->start(400);
}

void MessageStore::createSnapshot(QString dirname)
{
    // No work has to be done when there are no messages
    if (this->messages.empty()) {
        return;
    }

    for (auto it = this->messages.begin(); it != this->messages.end(); it++) {
        QDir dir;
        
        if (it.key()[0] == "/") {
            dir = QDir(dirname + "/<undefined-topic-name>" + it.key()); 
        } else {
            dir = QDir(dirname + "/" + it.key());
        }

        // Make sure the respective directory exists
        if (!dir.exists()) {
            // Skip this topic if the necessarry directories could not be made
            if (!dir.mkpath(dir.path())) {
                continue;
            }
        }

        QFile payload_file = QFile(dir.path() + "/payload.txt");

        // Open the payload file for writing. Skip in case of an error.
        if (!payload_file.open(QIODevice::WriteOnly)) {
            return;
        }

        // Get the latest value from a topic
        Message *msg = it.value()[it.value().count()-1];
        std::string payload = msg->getMessage();

        payload_file.write(payload.data(), payload.length());
    }
}

void MessageStore::setMessageCap(int cap)
{
    this->message_capacity = cap;
}

int MessageStore::getMessageCap()
{
    return this->message_capacity;
}

const QHash<QString, QList<Message*>> MessageStore::getAllMessages()
{
    return this->messages;
}

const QList<Message*> MessageStore::getTopicMessages(const QString topic)
{
    if (!this->messages.contains(topic)) {
        return QList<Message*>();
    }

    return this->messages[topic];
}

void MessageStore::addMessage(const mqtt::const_message_ptr msg, MessageType type)
{
    // This works for both scenarios: topic is known and topic is unknown.
    // Using the []operator to get an item causes the creation of a default
    // item than can be used to add the new message payload.
    auto qtopic = QString::fromStdString(msg->get_topic());
    QList<Message*> msg_list = this->new_messages[qtopic];

    Message* new_msg = new Message(msg, type);

    // Check if a received message matches a sent message by comparing the
    // payload and arrival date. A matching received message can be discarded.
    if (new_msg->msg_type == MessageType::RECEIVED) {
        QMutableListIterator<Message*> it(msg_list);
        it.toBack();
        while (it.hasPrevious()) {
            Message* msg = it.previous();
            std::chrono::duration<double> time_diff;

            time_diff = new_msg->arrival_date - msg->arrival_date;

            // The time difference between messages is too big. We can skip the
            // rest of the messages and add the new message.
            if (time_diff.count() > 1) {
                break;
            }

            // An already matched sent message was found. No unmatched messages
            // should be any further. We can skip the rest of the messages and
            // add the new message.
            if (msg->msg_type == MessageType::SENT_MATCHED) {
                break;
            }

            // No need to compare two received messages.
            if (msg->msg_type == MessageType::RECEIVED) {
                continue;
            }

            // Messages don't match -> definitely a different message
            if (msg->getMessage() != new_msg->getMessage()) {
                continue;
            }

            // We found a matching sent message within a specified time frame.
            // The received message is most likely an echo from the broker.
            msg->msg_type = MessageType::SENT_MATCHED;
            delete new_msg;
            return;
        }
    }

    msg_list.append(new_msg);

    this->new_messages.insert(qtopic, msg_list);
}

void MessageStore::handleTick()
{
    if (this->new_messages.empty()) {
        return;
    }

    auto new_messages = this->new_messages;

    this->new_messages.clear();
    for (auto i = new_messages.begin(); i != new_messages.end(); i++) {
        auto section = this->messages.take(i.key());

        section.append(i.value());

        this->messages.insert(i.key(), section);
    }

    for (auto i = this->messages.begin(); i != this->messages.end(); i++) {
        this->messages.insert(i.key(),
            i.value().mid(i.value().count() - this->message_capacity, -1));
    }

    emit this->newMessages(new_messages);
}
