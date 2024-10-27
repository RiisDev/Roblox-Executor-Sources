#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>

#include "widgets/Dashboard.h"
#include "widgets/Executor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    void setupDocks();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionabout_triggered();

    void on_actionAdd_Executor_triggered();

private:
    std::unique_ptr<Dashboard> dashboard;
    std::unique_ptr<Executor> executor;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
