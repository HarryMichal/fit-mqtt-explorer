#ifndef MESSAGESTORE_H
#define MESSAGESTORE_H

#include <QList>
#include <QObject>
#include <QString>
#include <QTimer>

#include "mqtt/message.h"

/**
 * @brief Type of a message
 */
enum MessageType {
    RECEIVED,
    SENT_UNMATCHED,
    SENT_MATCHED
};

/**
 * @brief Represents a message (either sent of received)
 */
class Message : public QObject
{
    Q_OBJECT

public:
    explicit Message(mqtt::const_message_ptr msg, MessageType type, QObject *parent = nullptr);

    MessageType msg_type; //<Whether message was received or sent and if it has been filtered
    std::chrono::time_point<std::chrono::steady_clock> arrival_date; //<Time of arrival of the message

    /**
     * @brief Returns the message payload
     */
    std::string getMessage();

private:
    mqtt::const_message_ptr msg;
};

class MessageStore : public QObject
{
    Q_OBJECT
public:
    explicit MessageStore(QObject *parent = nullptr);

    void setMessageCap(int cap);
    int getMessageCap();

    const QHash<QString, QList<Message*>> getAllMessages();
    const QHash<QString, QList<Message*>> getNewMessages();
    const QList<Message*> getTopicMessages(const QString topic);

signals:
    void newMessages(const QHash<QString, QList<Message*>>);

public slots:
    /**
     * @brief Adds a new message to the store
     * @details A message can be either sent or received. Due to the way the
     * MQTT protocol is made, it sends back to the sender their sent message.
     * To overcome the fact this method attempts to filter out such message.
     *
     * @param msg message
     * @param type type of the message
     */
    void addMessage(const mqtt::const_message_ptr msg, MessageType type = MessageType::RECEIVED);

private:
    QTimer *ticker;
    int message_capacity;
    QHash<QString, QList<Message*>> messages;
    QHash<QString, QList<Message*>> new_messages;

private slots:
    void handleTick();
};

#endif // MESSAGESTORE_H
