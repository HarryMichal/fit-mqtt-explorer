#include "mqtt/message.h"
#include "mqttmanager.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mqtt_manager(new MQTTManager),
    current_mode(ModeState::Dashboard),
    mode_selector(new QActionGroup(this))
{
    this->ui->setupUi(this);

    this->mqtt_manager->moveToThread(&worker_thread);
    this->worker_thread.start();

    MainWindow::setupStatusBar();
    MainWindow::setupActions();
}

MainWindow::~MainWindow()
{
    delete this->ui;
    delete this->mode_selector;

    this->worker_thread.quit();
    this->worker_thread.wait();
}

void MainWindow::setDashboardPage()
{
    if (this->current_mode != ModeState::Dashboard) {
        this->current_mode = ModeState::Dashboard;
        ui->mode_stack->setCurrentIndex(ModeState::Dashboard);
    }

    ui->actionHistory->setVisible(true);
    ui->actionActualState->setVisible(true);
}

void MainWindow::setExplorerPage()
{
    if (this->current_mode != ModeState::Explorer) {
        this->current_mode = ModeState::Explorer;
        ui->mode_stack->setCurrentIndex(ModeState::Explorer);
    }
    ui->actionHistory->setVisible(false);
    ui->actionActualState->setVisible(false);
}

void MainWindow::OpenConnectionWindow()
{
    this->new_connection_window = new NewConnection(this);
    this->new_connection_window->show();
}

void MainWindow::setupStatusBar()
{
    this->connection_status.setText("disconnected");
    this->ui->statusBar->addPermanentWidget(&this->connection_status);
}

void MainWindow::setupActions()
{
    // Couple mode selecting buttons into a group so that they are exclusive -> mode switcher
    this->mode_selector->addAction(ui->actionDashboard);
    this->mode_selector->addAction(ui->actionExplorer);
    this->mode_selector->setExclusive(true);

    // Basic signals
    connect(this->ui->actionDashboard, &QAction::triggered, this, &MainWindow::setDashboardPage);
    connect(this->ui->actionExplorer, &QAction::triggered, this, &MainWindow::setExplorerPage);

    // MQTT related signals
    connect(this->ui->actionConnect, &QAction::triggered, mqtt_manager, &MQTTManager::connect);
    connect(this->mqtt_manager, &MQTTManager::connectedChanged, this, &MainWindow::updateStatusBar);
    connect(this->mqtt_manager, &MQTTManager::messageReceived, this, &MainWindow::messageReceived);
    connect(this->ui->actionNewConnection, &QAction::triggered, this, &MainWindow::OpenConnectionWindow);
}

void MainWindow::updateStatusBar()
{
    if (this->mqtt_manager->connected) {
        this->connection_status.setText("connected");
    } else {
        this->connection_status.setText("disconnected");
    }
}

void MainWindow::messageReceived(const mqtt::const_message_ptr msg)
{
    qDebug("New message: %s", msg->get_payload_str().c_str());
}
