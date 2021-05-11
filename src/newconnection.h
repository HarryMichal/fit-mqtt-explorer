/**
 * @file newconnection.h
 * @author Žaneta Grossová <xgross11>
 */

#ifndef NEWCONNECTION_H
#define NEWCONNECTION_H

#include <QDialog>
#include <QString>

namespace Ui {
class newconnection;
}

/**
 * @brief The ModeState structure represents server adress
 * @details pre-prepared for saving connection form as structure,
 * now directly merged to string and not used
 */
struct ServerName {
    QString name;
    QString protocol;
    QString host;
    int port;
};
/**
 * @brief The NewConnection class represents pop-up window
 * with form for server adress
 */
class NewConnection : public QDialog
{
    Q_OBJECT

public:
    explicit NewConnection(QWidget *parent = nullptr);
    ~NewConnection();
    Ui::newconnection *ui;

signals:
    /**
     * @brief createNewConnection is emmited when user clicks on 'Connect' button when creating new connection
     * @param fullConnectionAdress merged form data into full server adress, format [host]:[port]
     */
    void createNewConnection(QString fullConnectionAdress);

private:

    /**
     * @brief setupActions prepare connections of signals to slots
     */
    void setupActions();
    void saveConnection();
    void loadConnections();
    void deleteConnection();
    /**
     * @brief processForm process filled data in NewConnection form, merge it into string end send it
     * as parameter of createNewConnection signal
     */
    void processForm();

};

#endif // NEWCONNECTION_H
