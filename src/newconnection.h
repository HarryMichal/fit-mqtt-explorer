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
 * @brief Represents server adress
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
 * @brief Represents dialog with form for server adress
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
     * @brief Is emmited when on 'Connect' button is clicked
     *
     * @param fullConnectionAdress merged form data into full server adress
     * @detail format [host]:[port]
     */
    void createNewConnection(QString fullConnectionAdress, QString client_id);

private:

    /**
     * @brief Set up signals and connections
     */
    void setupActions();
    void saveConnection();
    void loadConnections();
    void deleteConnection();
    /**
     * @brief Process filled data in NewConnection form, merge it into string end send it
     * as parameter of createNewConnection signal
     */
    void processForm();

};

#endif // NEWCONNECTION_H
