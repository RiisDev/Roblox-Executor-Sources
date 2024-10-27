#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <QMainWindow>
#include <QDockWidget>
#include <QWebEngineView>

#include <memory>

namespace Ui {
class Executor;
}

class Executor : public QDockWidget
{
    Q_OBJECT

public:
    explicit Executor(QMainWindow *parent = nullptr);
    ~Executor();

private:
    std::unique_ptr<QWebEngineView> view;
    Ui::Executor *ui;
};

#endif // EXECUTOR_H
