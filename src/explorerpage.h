#ifndef EXPLORERPAGE_H
#define EXPLORERPAGE_H

#include <QList>
#include <QListWidgetItem>
#include <QWidget>
#include <QStandardItemModel>
#include <QString>
#include <QTreeView>

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

public slots:
    void changeSelectedMessage(QListWidgetItem *current, QListWidgetItem *previous);
    void changeSelectedTopic(const QModelIndex &current);
    void initConnection(const QString server_name);
    void receiveNewMessages(const QHash<QString, QList<QString>> new_msgs);
    void setMessage(QString msg);
    void setTopic(QList<QString>);

private:
    Ui::ExplorerPage *ui;
    QStandardItemModel topics_tree_model;

    int message_capacity;

    void addTopic(const QString topic, const bool root);
};

#endif // EXPLORERPAGE_H
