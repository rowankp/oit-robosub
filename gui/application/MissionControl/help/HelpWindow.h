#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QDialog>

namespace Ui {
class helpwindow;
}

class HelpWindow : public QDialog
{
    Q_OBJECT

public:
    explicit HelpWindow(QWidget *parent = nullptr);
    ~HelpWindow();
private slots:
    void handleDragDrop();
    void handleParameters();
private:
    Ui::helpwindow *ui;
};

#endif // HELPWINDOW_H
