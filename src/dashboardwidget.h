#ifndef DASHBOARDWIDGET_H
#define DASHBOARDWIDGET_H

#include <QWidget>

#include "editwidget.h"
#include "messagestore.h"
#include "mqtt/message.h"

namespace Ui {
class dashboardWidget;
}

/**
 * @brief The widgetStructure struct represents settings of current widget
 */
struct widgetStructure{
    QString name;
    QString topic;
    QString type;
    bool setpoint;
    QString setpointValue;
};

/**
 * @brief Represents one widget
 */
class DashboardWidget : public QWidget
{
    Q_OBJECT

public:

    explicit DashboardWidget(QWidget *parent = nullptr);
    ~DashboardWidget();

    /**
     * @brief Is called when new messages are sent
     *
     * @param Hash table with new messages
     *
     */
    void UpdateNewValue(const QHash<QString, QList<Message*>> new_msgs);

private:

    /**
     * @brief Set global values and connections to slots
     */
    void setupActions();
    /**
     * @brief Removes widget
     */
    void RemoveWidget();
    /**
     * @brief Show dialog for edit widget
     */
    void EditWidget();
    /**
     * @brief Apply changes from edit dialog
     *
     * @param Structure witch changes to apply
     *
     */
    void ApplyChanges(widgetStructure widgetStructure);
    /**
     * @brief Set format of value due to selected type of widget
     *
     */
    void setValueFormat();

    Ui::dashboardWidget *ui;
    class EditWidget *editWidget;
};

#endif // DASHBOARDWIDGET_H
