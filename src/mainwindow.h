#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QActionGroup>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum ModeState {
    Dashboard,
    Explorer
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void SetDashboardPage();
    void SetExplorerPage();

private:
    Ui::MainWindow *ui;

    ModeState currentMode;
    QActionGroup *modeSelector;

    void setupActions();
};

#endif // MAINWINDOW_H
