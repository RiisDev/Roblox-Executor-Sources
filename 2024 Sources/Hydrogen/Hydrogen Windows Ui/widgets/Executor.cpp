#include "Executor.h"
#include "ui_Executor.h"

#include <QHBoxLayout>

#include <QTextEdit>

Executor::Executor(QMainWindow *parent) :
    QDockWidget(parent),
    ui(new Ui::Executor)
{
    ui->setupUi(this);

    view = std::make_unique<QWebEngineView>();
    view->load(QUrl::fromLocalFile(qApp->applicationDirPath() + "/monaco/index.html"));

    this->setWidget(view.get());
}

Executor::~Executor()
{
    delete ui;
}

