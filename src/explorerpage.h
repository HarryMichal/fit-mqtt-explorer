#ifndef EXPLORERPAGE_H
#define EXPLORERPAGE_H

#include <QWidget>
#include <QStandardItemModel>

#include "mqtt/message.h"

namespace Ui {
class ExplorerPage;
}

class ExplorerPage : public QWidget
{
    Q_OBJECT

public:
    explicit ExplorerPage(QWidget *parent = nullptr);
    ~ExplorerPage();

public slots:
    void initConnection(const QString server_name);
    void receiveNewMessages(const QHash<QString, QList<QString>> new_msgs);

private:
    Ui::ExplorerPage *ui;
    QStandardItemModel topics_tree_model;

    void addTopic(const QString topic, const bool root);
};

#endif // EXPLORERPAGE_H
