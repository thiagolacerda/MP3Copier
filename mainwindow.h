#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include "mythread.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void copyClicked();
    void clearSelections();
    void selectionChanged();
    void cancelCopying();
    void copyThreadFinished();

private:
    void init();

    Ui::MainWindow *ui;
    QFileSystemModel *fileSystem;
    MyThread *myThread;
};

#endif // MAINWINDOW_H
