//******************************************************************************
// filename: MainWindow.h
// authors: Rowan Parker (rowan.parker@oit.edu)
// date: 02/17/2020
// purpose: main application space.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtCore/QStringListModel>
#include <QtWidgets/QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow() override;

    protected:
        void initActionsAvailableModel();
        void initColorButtons();
        void initShapeButtons();

    private:
        Ui::MainWindow   *_ui;
        QString _targetColor = "UNDEFINED";
        QString _targetShape = "UNDEFINED";
        QString _targetRadius = 0;
        QString _targetLength = 0;
        QString _targetWidth = 0;

    private slots:
        void help();
        void about();
        void confirmMission();
        void exportMission();
        void cancelMission();
        void showDimensions();
        void showUnsupported();
        void addTask(QListWidgetItem *task);
        void removeTask(QListWidgetItem *task);
        void setTargetColor(const QString &color);
        void setTargetShape(const QString &shape);
        void setTargetRadius(const QString &radius);
        void setTargetLength(const QString &length);
        void setTargetWidth(const QString &width);
};
#endif // MAINWINDOW_H
