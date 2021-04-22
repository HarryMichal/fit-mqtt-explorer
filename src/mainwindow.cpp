#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentMode(ModeState::Dashboard),
    modeSelector(new QActionGroup(this))
{
    this->ui->setupUi(this);

    MainWindow::setupActions();
}

MainWindow::~MainWindow()
{
    delete this->ui;
    delete this->modeSelector;
}

void MainWindow::SetDashboardPage()
{
    if (this->currentMode != ModeState::Dashboard) {
        this->currentMode = ModeState::Dashboard;
        ui->mode_stack->setCurrentIndex(ModeState::Dashboard);
    }
}

void MainWindow::SetExplorerPage()
{
    if (this->currentMode != ModeState::Explorer) {
        this->currentMode = ModeState::Explorer;
        ui->mode_stack->setCurrentIndex(ModeState::Explorer);
    }
}

void MainWindow::setupActions()
{
    // Couple mode selecting buttons into a group so that they are exclusive -> mode switcher
    this->modeSelector->addAction(ui->actionDashboard);
    this->modeSelector->addAction(ui->actionExplorer);
    this->modeSelector->setExclusive(true);

    // Connect signals to slots
    connect(this->ui->actionDashboard, &QAction::triggered, this, &MainWindow::SetDashboardPage);
    connect(this->ui->actionExplorer, &QAction::triggered, this, &MainWindow::SetExplorerPage);
}
