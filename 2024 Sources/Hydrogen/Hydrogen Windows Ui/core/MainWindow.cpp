#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include "dialogs/AboutDialog.h"



void MainWindow::setupDocks()
{
    dashboard = std::make_unique<Dashboard>(this);
    dashboard->setAllowedAreas(Qt::AllDockWidgetAreas);
    addDockWidget(Qt::DockWidgetArea::TopDockWidgetArea, dashboard.get());

    ui->menuWindows->addAction(dashboard->toggleViewAction());

//    executor = std::make_unique<Executor>(this);
//    executor->setAllowedAreas(Qt::AllDockWidgetAreas);
//    addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, executor.get());

//    ui->menuWindows->addAction(executor->toggleViewAction());
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupDocks();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionabout_triggered()
{
    AboutDialog *a = new AboutDialog(this);
    a->setAttribute(Qt::WA_DeleteOnClose);
    a->open();
}


void MainWindow::on_actionAdd_Executor_triggered()
{
    const auto executor = new Executor(this);
    executor->setAllowedAreas(Qt::AllDockWidgetAreas);
    executor->show();
    addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, executor);
}

