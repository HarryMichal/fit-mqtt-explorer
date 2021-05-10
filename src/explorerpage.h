#ifndef EXPLORERPAGE_H
#define EXPLORERPAGE_H

#include <QList>
#include <QListWidgetItem>
#include <QWidget>
#include <QStandardItemModel>
#include <QString>
#include <QTreeView>

#include "messagestore.h"
#include "mqtt/message.h"

namespace Ui {
class ExplorerPage;
}

class MessageTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit MessageTreeView(QWidget *parent = nullptr) : QTreeView(parent) {};

signals:
    void onCurrentChanged(const QModelIndex &current);

private:
    virtual void currentChanged(const QModelIndex &current, const QModelIndex &previous);
};

class ExplorerPage : public QWidget
{
    Q_OBJECT

public:
    explicit ExplorerPage(QWidget *parent = nullptr, int cap = 50);
    ~ExplorerPage();

    QString current_topic; //<currently selected topic

    void setMessageCap(int cap);

signals:
    void onChangeSelectedMessage(const int currentRow);
    void onChangeSelectedTopic(QString topic);
    /**
     * @brief onSendFile is emitted when a file is to be sent
     *
     * @param topic topic where the message should be sent
     * @param file_name name of the file to be sent
     */
    void onSendFile(mqtt::string topic, QString file_name);
    /**
     * @brief onSendText is emitted when a plain message is to be sent
     *
     * @param topic topic where the message should be sent
     * @param msg message to be sent 
     */
    void onSendText(mqtt::string topic, mqtt::string msg);

public slots:
    void changeSelectedMessage(QListWidgetItem *current, QListWidgetItem *previous);
    void changeSelectedTopic(const QModelIndex &current);
    void initConnection(const QString server_name);
    void receiveNewMessages(const QHash<QString, QList<Message*>> new_msgs);
    /**
     * @brief selectFile() spawns a QFileDialog, gets the selected file name
     * and sets it to relevant QLineEdit
     */
    void selectFile();
    /**
     * @brief sendMessage() serves as an entry-point for sending messages from
     * ExplorerPage
     * @details Depending on the selected radio button it emits the right
     * signal, gathers needed info from the GUI and also allows for sending
     * messages to a different subtopic.
     */
    void sendMessage();
    void setMessage(Message *msg);
    void setTopic(QList<Message*>);

private:
    Ui::ExplorerPage *ui;
    QStandardItemModel topics_tree_model;

    int message_capacity;

    /**
     * @brief addMessages() adds strings to the message history list
     * @details If a message is longer than 26 characters, it is truncated and
     * three dots at the end are added to signify the truncation. Every message
     * is parsed into a pixmap to try to see if there is an image (JPG/PNG).
     * The message capacity is respected by deleting the oldest messages in the
     * list.
     */
    void addMessages(QList<Message*> msgs);
    void addTopic(const QString topic, const bool root);
};

#endif // EXPLORERPAGE_H
