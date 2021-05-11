#ifndef EDITWIDGET_H
#define EDITWIDGET_H

#include <QDialog>

#include "dashboardwidget.h"
#include "messagestore.h"

namespace Ui {
class EditWidget;
}

/**
 * @brief Represents dialog for editing widget
 */
class EditWidget : public QDialog
{
    Q_OBJECT

public:
    explicit EditWidget(struct widgetStructure widgetStructure, QWidget *parent = nullptr);
    ~EditWidget();

signals:
    /**
     * @brief Is emitted when editting widget is done
     *
     * @param structure with set settings
     */
    void applyChangesOnWidget(struct widgetStructure widgetStructure);

private:
    Ui::EditWidget *ui;
    MessageStore *msg_store; //<Store of messages from a MQTT broker

    /**
     * @brief Set up global actions
     *
     * @param Structure with set values from widget
     */
    void setupActions(struct widgetStructure widgetStructure);
    /**
     * @brief When setpoint is enabled, it sets visible
     * line for setting setpoint
     */
    void checkBoxStatusChanged();
    /**
     * @brief Saves values grom form into structure and send it back to widget
     */
    void setWidgetSettings();

};

#endif // EDITWIDGET_H
