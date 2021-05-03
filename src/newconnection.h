#ifndef NEWCONNECTION_H
#define NEWCONNECTION_H

#include <QDialog>
#include <QString>

namespace Ui {
class newconnection;
}

/**
 * @brief The ServerName structure represents server adress
 */
struct ServerName {
    QString name;
    QString protocol;
    QString host;
    int port;
};

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

    void setupActions();
    void saveConnection();
    void loadConnections();
    void deleteConnection();
    QString connectToContent();

};

#endif // NEWCONNECTION_H
