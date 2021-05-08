#ifndef MESSAGEHISTORYDIALOG_H
#define MESSAGEHISTORYDIALOG_H

#include <QDialog>

namespace Ui {
class MessageHistoryDialog;
}

/**
 * @brief The MessageHistoryDialogs class represents a dialog window for
 * setting message history limit of the app
 */
class MessageHistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MessageHistoryDialog(QWidget *parent = nullptr);
    ~MessageHistoryDialog();

    /**
     * @brief getValue() returns the set value in the dialog
     */
    int getValue();
    /**
     * @brief setValue() sets the value used in the dialog. Useful for setting
     * the current value.
     */
    void setValue(int value);
private:
    Ui::MessageHistoryDialog *ui;
};

#endif // MESSAGEHISTORYDIALOG_H
