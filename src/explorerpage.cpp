#include <QHash>
#include <QString>

#include "explorerpage.h"
#include "ui_explorerpage.h"

ExplorerPage::ExplorerPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExplorerPage)
{
    ui->setupUi(this);

    ui->topicsTree->setModel(&this->topics_tree_model);
}

ExplorerPage::~ExplorerPage()
{
    delete ui;
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
