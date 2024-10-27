#include "Dashboard.h"
#include "ui_Dashboard.h"

Dashboard::Dashboard(QMainWindow *parent) :
    QDockWidget(parent),
    ui(new Ui::Dashboard)
{
    ui->setupUi(this);
}

Dashboard::~Dashboard()
{
    delete ui;
}
