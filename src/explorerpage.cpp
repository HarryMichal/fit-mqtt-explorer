/**
 * @file explorerpage.cpp
 * @author Ondřej Míchal <xmicha80>
 */

#include <QBrush>
#include <QColor>
#include <QFileDialog>
#include <QHash>
#include <QPixmap>
#include <QPushButton>
#include <QStandardPaths>
#include <QString>

#include "explorerpage.h"
#include "ui_explorerpage.h"

void MessageTreeView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    emit this->onCurrentChanged(current);
}

ExplorerPage::ExplorerPage(QWidget *parent, int cap) :
    QWidget(parent),
    ui(new Ui::ExplorerPage),
    message_capacity(cap)
{
    ui->setupUi(this);

    ui->topicsTree->setModel(&this->topics_tree_model);

    connect(ui->fileChooseButton, &QPushButton::clicked, this, &ExplorerPage::selectFile);
    connect(ui->messageHistoryList, &QListWidget::currentItemChanged, this, &ExplorerPage::changeSelectedMessage);
    connect(ui->sendButton, &QPushButton::clicked, this, &ExplorerPage::sendMessage);
    connect(ui->topicsTree, &MessageTreeView::onCurrentChanged, this, &ExplorerPage::changeSelectedTopic);
}

ExplorerPage::~ExplorerPage()
{
    delete ui;
}

void ExplorerPage::setMessageCap(int cap)
{
    this->message_capacity = cap;
}

void ExplorerPage::changeSelectedMessage(QListWidgetItem *current, QListWidgetItem *previous)
{
    auto curr_row = this->ui->messageHistoryList->row(current);
    auto prev_row = this->ui->messageHistoryList->row(previous);

    // No need to refresh the view when no other item is selected.
    if (curr_row == prev_row) {
        return;
    }

    // Hack alert: In order to prevent losing the preview of a message that
    // might have been deleted from the history list (very likely when the
    // frequency of messages is very high) put in place this hack. It skips the
    // setting of a new message preview on overflow.
    if (this->ui->messageHistoryList->count() >= this->message_capacity) {
        if (curr_row == this->message_capacity-1 && prev_row == this->message_capacity) {
            return;
        }
    }

    emit this->onChangeSelectedMessage(curr_row);
}

void ExplorerPage::changeSelectedTopic(const QModelIndex &current)
{
    QString topic = "";

    // Assemble the topic name
    auto item = this->topics_tree_model.itemFromIndex(current);
    for (; item != nullptr; item = item->parent()) {
        topic.prepend(item->text());
        if (item->parent() != 0) {
            topic.prepend("/");
        }
    }

    // The root is always the name of the server. It is not part of the topic name.
    topic = topic.mid(topic.indexOf("/") + 1);

    this->ui->topicNameLabel->setText(topic);
    this->current_topic = topic;

    this->ui->messageHistoryList->setCurrentRow(-1);
    emit onChangeSelectedTopic(topic);
}

void ExplorerPage::initConnection(const QString server_name)
{
    this->topics_tree_model.clear();
    this->addTopic(server_name, true);
}

void ExplorerPage::receiveNewMessages(const QHash<QString, QList<Message*>> new_msgs)
{
    QHash<QString, QList<Message*>>::const_iterator i = new_msgs.constBegin();
    for (; i != new_msgs.constEnd(); i++) {
        auto topic = i.key();
        this->addTopic(topic, false);
    }

    // Don't touch the message history list if no relevant new messages arrived
    if (!new_msgs.contains(this->current_topic)) {
        return;
    }
    
    this->addMessages(new_msgs[this->current_topic]);
}

void ExplorerPage::selectFile()
{
    auto file_name = QFileDialog::getOpenFileName(this, "Choose file", QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    this->ui->fileNameLineEdit->setText(file_name);
}

void ExplorerPage::sendMessage()
{
    auto topic = this->ui->topicNameLabel->text().toStdString();

    // Send the message to a subtopic
    // TODO: Subscribe to the subtopic
    if (this->ui->splitTopicBox->isChecked()) {
        topic += "/" + this->ui->splitTopicText->text().toStdString();
    }

    // Send plain text message
    if (this->ui->radioButtonPlainText->isChecked()) {
        auto msg = this->ui->messageTextEdit->toPlainText().toStdString();

        emit this->onSendText(topic, msg);
    }

    // Send binary message (file)
    if (this->ui->radioButtonFile->isChecked()) {
        auto file_name = this->ui->fileNameLineEdit->text();

        emit this->onSendFile(topic, file_name);
    }
}

void ExplorerPage::setMessage(Message *msg)
{
    this->ui->messagePreview->setText(QString::fromStdString(msg->getMessage()));
}

void ExplorerPage::setTopic(QList<Message*> topic_msgs)
{
    this->ui->messageHistoryList->clear();
    this->addMessages(topic_msgs);
}

void ExplorerPage::addMessages(QList<Message*> msgs)
{
    for (auto it = msgs.begin(); it != msgs.end(); it++) {
        std::string msg = (*it)->getMessage();
        QByteArray data(msg.c_str(), msg.length());
        QListWidgetItem *new_item = new QListWidgetItem;
        QPixmap pixmap;

        if (pixmap.loadFromData(data, "JPG") || pixmap.loadFromData(data, "PNG")) {
            // The payload is a pixmap (picture)
            msg = "<binary-message::picture>";
        } else {
            // Truncate messages longer than a certain number of characters
            // If truncated, add three dots to signalize it.

            if (msg.length() > 26) {
                msg.resize(26);
                msg += "...";
            }
        }

        new_item->setText(QString::fromStdString(msg));
        // Based on message type set background color. I took some colors that
        // looked nice in Krita.
        if ((*it)->msg_type == MessageType::RECEIVED) {
            new_item->setBackground(QBrush(QColor(184, 244, 245)));
        } else {
            new_item->setBackground(QBrush(QColor(255, 125, 125)));
        }

        this->ui->messageHistoryList->insertItem(0, new_item);

        // Respect the set message capacity
        for (int i = this->ui->messageHistoryList->count(); i > this->message_capacity; i--) {
            delete this->ui->messageHistoryList->takeItem(i-1);
        }
    }
}

void ExplorerPage::addTopic(const QString topic, const bool root)
{
    QStandardItem *parent = this->topics_tree_model.invisibleRootItem();

    // The root node is always called the same as the server. The rest of the
    // nodes are always a child of this node.
    if (!root) {
        parent = parent->child(0);
    }

    // To build up the tree, we need to check every parent topic in the chain.
    auto topics = topic.split("/");

    for (QList<QString>::iterator i = topics.begin(); i != topics.end(); i++) {
        bool found = false;

        // If we want to add an item, we need to find its parent first. The
        // item is then added to it.
        for (int row = 0; row < parent->rowCount(); row++) {
            if (*i != parent->child(row)->text()) {
                continue;
            }

            // Found topic with the same name -> new parent
            parent = parent->child(row);
            found = true;
            break;
        }

        if (found) {
            continue;
        }

        QStandardItem *new_topic_node = new QStandardItem();
        new_topic_node->setText(*i);
        parent->appendRow(new_topic_node);
        parent = new_topic_node;
        this->topics_tree_model.sort(0);
    }
}
