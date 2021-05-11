/**
 * @file messagestore.h
 * @author Ondřej Míchal <xmicha80>
 */

#ifndef MESSAGESTORE_H
#define MESSAGESTORE_H

#include <QHash>
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

    MessageType msg_type; //< Whether message was received or sent and if it has been filtered
    std::chrono::time_point<std::chrono::steady_clock> arrival_date; //< Time of arrival of the message

    /**
     * @brief Returns the message payload
     */
    std::string getMessage();

private:
    mqtt::const_message_ptr msg; //< Underlying message
};

/**
 * @brief Represents a storage of MQTT messages (both received & sent)
 * @details Messages are handled using two hash maps: messages & new_messages.
 * messages hash map serves for accumulating all messages and new_messages for
 * accumulating new messages that are than propagated using the newMessages
 * signal. This gives the option to alter cadency of delivery.
 */
class MessageStore : public QObject
{
    Q_OBJECT
public:
    explicit MessageStore(QObject *parent = nullptr);

    /**
     * @brief createSnapshot() saves the latest message of all topics into
     * a specified location
     * @details Inside of the user-specified directory will be messages for all
     * topics. They will not be put into a parent directory called after e.g.,
     * the server. The file structure will mimic the tree structure of the
     * topics. If a name in the chain of topic names is unknown, it is replaced
     * by "<undefined-topipc-name>".
     */
    void createSnapshot(QString dirname);
    /**
     * @brief Sets maximum message capacity
     *
     * @param cap max number of messages
     */
    void setMessageCap(int cap);
    /**
     * @brief Returns set maximum message capacity
     */
    int getMessageCap();
    /**
     * @brief Returns all "commited" messages
     */
    const QHash<QString, QList<Message*>> getAllMessages();
    /**
     * @brief Returns all "new" messages
     */
    const QHash<QString, QList<Message*>> getNewMessages();
    /**
     * @brief Returns all messages from a topic
     */
    const QList<Message*> getTopicMessages(const QString topic);

signals:
    /**
     * @brief Is emitted when internal timeout has been reached and there are
     * new messages
     *
     * @param msgs new messages
     */
    void newMessages(const QHash<QString, QList<Message*>> msgs);

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
    QTimer *ticker; //< Heartbeat of new message delivery
    int message_capacity; //< Max number of stored messages
    QHash<QString, QList<Message*>> messages; //< "commited" messages (stores all messages)
    QHash<QString, QList<Message*>> new_messages; //< new messages (not "commited" yet)

private slots:
    /**
     * @brief Prepares the publishing of new messages
     */
    void handleTick();
};

#endif // MESSAGESTORE_H
