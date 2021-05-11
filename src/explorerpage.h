/**
 * @file explorerpage.h
 * @author Ondřej Míchal <xmicha80>
 */

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

/**
 * @brief Represents a tree of topics
 */
class MessageTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit MessageTreeView(QWidget *parent = nullptr) : QTreeView(parent) {};

signals:
    /**
     * @brief Is emitted when a new topic has been selected
     *
     * @param current model index of currently selected topic
     */
    void onCurrentChanged(const QModelIndex &current);

private:
    /**
     * @brief Handles the change of selected topic
     */
    virtual void currentChanged(const QModelIndex &current, const QModelIndex &previous);
};

/**
 * @brief Represents page for browsing & interacting with topics
 */
class ExplorerPage : public QWidget
{
    Q_OBJECT

public:
    explicit ExplorerPage(QWidget *parent = nullptr, int cap = 50);
    ~ExplorerPage();

    QString current_topic; //< Currently selected topic

    /**
     * @brief Sets the maximum message capacity
     *
     * @param cap maximum number of messages
     */
    void setMessageCap(int cap);

signals:
    /**
     * @brief Is emitted when a message is selected in the message history list
     *
     * @param currentRow selected row
     */
    void onChangeSelectedMessage(const int currentRow);
    /**
     * @brief Is emitted when a topic is selected in the topic tree view
     *
     * @param topic selected topic
     */
    void onChangeSelectedTopic(QString topic);
    /**
     * @brief Is emitted when a file is to be sent
     *
     * @param topic topic where the message should be sent
     * @param file_name name of the file to be sent
     */
    void onSendFile(mqtt::string topic, QString file_name);
    /**
     * @brief Is emitted when a plain message is to be sent
     *
     * @param topic topic where the message should be sent
     * @param msg message to be sent 
     */
    void onSendText(mqtt::string topic, mqtt::string msg);

public slots:
    /**
     * @brief Handles changing of selected messages in the message history list
     *
     * @param current currently selected message
     * @param previous previously selected message
     */
    void changeSelectedMessage(QListWidgetItem *current, QListWidgetItem *previous);
    /**
     * @brief Handles changing of selected topics
     *
     * @param current model index of currently selected topic
     */
    void changeSelectedTopic(const QModelIndex &current);
    /**
     * @brief Clears the explorer and prepares it for a new connection 
     *
     * @param server_name name of the server
     */
    void initConnection(const QString server_name);
    /**
     * @brief Handles receiving of new batch of messages
     * @details Populates both the topic tree view and message history list of
     * the currently selected topic
     *
     * @param new_msgs new messages
     */
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
    /**
     * @brief Populates message preview with message payload
     *
     * @param msg message
     */
    void setMessage(Message *msg);
    /**
     * @brief Populates message history list with relevant messages
     *
     * @param topic_msgs topic messages
     */
    void setTopic(QList<Message*> topic_msgs);

private:
    Ui::ExplorerPage *ui;
    QStandardItemModel topics_tree_model; //< Item model for topics

    int message_capacity; //< Maximum message capacity in message history list

    /**
     * @brief Adds strings to the message history list
     * @details If a message is longer than 26 characters, it is truncated and
     * three dots at the end are added to signify the truncation. Every message
     * is parsed into a pixmap to try to see if there is an image (JPG/PNG).
     * The message capacity is respected by deleting the oldest messages in the
     * list.
     *
     * @param msgs messages
     */
    void addMessages(QList<Message*> msgs);
    /**
     * @brief Adds a new topic to topics tree model
     *
     * @param topic new topic
     * @param root if the new topic should be a root node
     */
    void addTopic(const QString topic, const bool root);
};

#endif // EXPLORERPAGE_H
