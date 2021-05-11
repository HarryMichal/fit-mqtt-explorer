/**
 * @file dashboardpage.h
 * #author Žaneta Grossová <xgross11>
 */

#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QWidget>
#include <QList>
#include <QtWidgets>
#include "dashboardwidget.h"

namespace Ui {
class DashboardPage;
}

/**
 * @brief Represents page for simple data showing via widgets
 */
class DashboardPage : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardPage(QWidget *parent = nullptr);
    ~DashboardPage();

    /**
     * @brief adds new widget in dashboard layout
     */
    void AddNewWidget();

public slots:

    /**
     * @brief receiveNewMessages() when new messages arrive, main window send
     * signal to this slot
     * @param new_msgs hash table with new messages
     */
    void receiveNewMessages(const QHash<QString, QList<Message*>> new_msgs);

private:

    /**
     * @brief setupActions() connect pre-prepared layout to
     * code for inserting widgets
     */
    void setupActions();

    Ui::DashboardPage *ui;
    QVBoxLayout* layout;
    DashboardWidget* dashboardWidget;
    QList<QWidget*> widgetList;
};

#endif // DASHBOARDPAGE_H
