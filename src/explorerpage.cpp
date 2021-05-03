#include <QHash>
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

    connect(ui->messageHistoryList, &QListWidget::currentItemChanged, this, &ExplorerPage::changeSelectedMessage);
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

    if (curr_row == prev_row) {
        return;
    }

    if (curr_row == 1 && prev_row == 0 && this->ui->messageHistoryList->count() >= this->message_capacity) {
        return;
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

void ExplorerPage::receiveNewMessages(const QHash<QString, QList<QString>> new_msgs)
{
    QHash<QString, QList<QString>>::const_iterator i = new_msgs.constBegin();
    for (; i != new_msgs.constEnd(); i++) {
        auto topic = i.key();
        this->addTopic(topic, false);
    }

    // Don't touch the message history list if no relevant new messages arrived
    if (!new_msgs.contains(this->current_topic)) {
        return;
    }

    this->ui->messageHistoryList->addItems(new_msgs[this->current_topic]);
    // Respect the set message capacity
    for (int i = this->ui->messageHistoryList->count(); i > this->message_capacity; i--) {
        delete this->ui->messageHistoryList->takeItem(0);
    }
}

void ExplorerPage::setMessage(const QString msg)
{
    this->ui->messagePreview->setText(msg);
}

void ExplorerPage::setTopic(const QList<QString> topic_msgs)
{
    this->ui->messageHistoryList->clear();
    this->ui->messageHistoryList->addItems(topic_msgs);
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
