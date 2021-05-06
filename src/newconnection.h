#ifndef NEWCONNECTION_H
#define NEWCONNECTION_H

#include <QDialog>

namespace Ui {
class newconnection;
}

class NewConnection : public QDialog
{
    Q_OBJECT

public:
    explicit NewConnection(QWidget *parent = nullptr);
    ~NewConnection();
    Ui::newconnection *ui;

signals:
    void createNewConnection();

private:

    void setupSignals();
};

#endif // NEWCONNECTION_H
