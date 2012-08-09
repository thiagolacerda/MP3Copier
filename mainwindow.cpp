#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <iostream>
#include <QThreadPool>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    //toCopy = false;
    myThread = new MyThread();
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(copyClicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(clearSelections()));
    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(selectionChanged()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(cancelCopying()));
    connect(myThread, SIGNAL(finished()), this, SLOT(copyThreadFinished()));
    //ui->label->setVisible(false);
    fileSystem = new QFileSystemModel(this);
    fileSystem->setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden | QDir::Drives);
    fileSystem->setRootPath("/");
    ui->treeView->setModel(fileSystem);
    ui->treeView->setRootIndex(fileSystem->index("/"));//Users/thiagolacerda/Music/iTunes/iTunes Music/Music"));
    ui->treeView->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeView_2->setModel(fileSystem);
    ui->pushButton_3->setEnabled(false);
    myThread->srcTree = ui->treeView;
    myThread->destTree = ui->treeView_2;
    myThread->fileSystem = fileSystem;
    myThread->sizeLabel = ui->label;
}

void MainWindow::copyClicked()
{
    myThread->toCopy = true;
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(true);
    myThread->start();
}

void MainWindow::selectionChanged()
{
    myThread->toCopy = false;
    if (!myThread->isRunning()) {
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        myThread->start();
    }
}

void MainWindow::clearSelections()
{
    ui->treeView->clearSelection();
    ui->label->setText("");
}

void MainWindow::cancelCopying()
{
    if (myThread->isRunning()) {
        myThread->terminate();
        //ui->pushButton->setEnabled(true);
    }
    ui->pushButton_3->setEnabled(false);
}

void MainWindow::copyThreadFinished()
{
    ui->pushButton->setEnabled(true);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
}

