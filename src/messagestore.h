#ifndef MESSAGESTORE_H
#define MESSAGESTORE_H

#include <QList>
#include <QObject>
#include <QString>
#include <QTimer>

#include "mqtt/message.h"

class MessageStore : public QObject
{
    Q_OBJECT
public:
    explicit MessageStore(QObject *parent = nullptr);

    void setMessageCap(int cap);
    int getMessageCap();

    const QHash<QString, QList<QString>> getAllMessages();
    const QHash<QString, QList<QString>> getNewMessages();
    const QList<QString> getTopicMessages(const mqtt::string topic);

signals:
    void newMessages(const QHash<QString, QList<QString>>);

public slots:
    void addMessage(const mqtt::const_message_ptr msg);

private:
    QTimer *ticker;
    int message_capacity;
    QHash<QString, QList<QString>> messages;
    QHash<QString, QList<QString>> new_messages;

private slots:
    void handleTick();
};

#endif // MESSAGESTORE_H
