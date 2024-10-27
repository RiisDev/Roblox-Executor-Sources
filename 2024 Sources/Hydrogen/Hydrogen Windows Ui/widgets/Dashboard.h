#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QMainWindow>
#include <QDockWidget>

namespace Ui {
class Dashboard;
}

class Dashboard : public QDockWidget
{
    Q_OBJECT

public:
    explicit Dashboard(QMainWindow *parent = nullptr);
    ~Dashboard();

private:
    Ui::Dashboard *ui;
};

#endif // DASHBOARD_H
