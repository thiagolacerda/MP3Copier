#include "mythread.h"
#include <QModelIndexList>
#include <iostream>

using namespace std;

MyThread::MyThread(QObject *parent) :
    QThread(parent)
{
}

void MyThread::copyMP3s(QString path, QDir &dest)
{
    QDir dir(path);
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
    QFileInfoList l = dir.entryInfoList();
    QFileInfo f;
    QString tempPath;
    QString destFile;
    for (int i = 0; i < l.size(); ++i) {
        f = l.at(i);
        tempPath = f.filePath();
        if (f.isDir())
            copyMP3s(tempPath, dest);
        else if(QDir::match("*.mp3", f.fileName())) {
            QString label = "Copying: ";
            label.append(f.fileName());
            sizeLabel->setText(label);
            destFile = dest.absolutePath();
            destFile.append("/");
            destFile.append(f.fileName());
            if (QFile::copy(tempPath, destFile)) {
                cout << "File " << f.fileName().toStdString() << " copied to " << destFile.toStdString() << endl;
            } else {
                cout << "Could not copy " << f.fileName().toStdString() << ". Maybe file already exists in " << destFile.toStdString() << endl;
            }
        }
    }
}

qint64 MyThread::calculateTotalSize(QDir &dir)
{
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
    QFileInfoList l = dir.entryInfoList();
    QFileInfo fileInfo;
    qint64 totalSize = 0;
    for (int i = 0; i < l.size(); ++i) {
        fileInfo = l.at(i);
        if (fileInfo.isDir()) {
            QDir temp(fileInfo.absoluteFilePath());
            totalSize += calculateTotalSize(temp);
        } else {
            totalSize += fileInfo.size();
        }
    }
    return totalSize;
    //total = (((double)totalSize)/1024.0)/1024.0;
    //ui->label->setText(QString("Total Size: %1MB").arg(total));
}

void MyThread::updateTotalSize()
{
    QModelIndexList l = srcTree->selectionModel()->selectedRows();
    QFileInfo fileInfo;
    qint64 totalSize = 0;
    double total = 0;
    sizeLabel->setText("Wait, calculating total size");
    for (int i = 0; i < l.size(); ++i) {
        fileInfo = fileSystem->fileInfo(srcTree->selectionModel()->selectedRows().at(i));
        if (fileInfo.isDir()) {
            QDir temp(fileInfo.absoluteFilePath());
            totalSize += calculateTotalSize(temp);
        } else {
            totalSize += fileInfo.size();
        }
    }
    total = (((double)totalSize)/1024.0)/1024.0;
    sizeLabel->setText(QString("Total Size: %1MB").arg(total));
}

void MyThread::startCopying()
{
    QModelIndexList l = srcTree->selectionModel()->selectedRows();
    QFileInfo sourceFileInfo;
    QFileInfo destFileInfo = fileSystem->fileInfo(destTree->selectionModel()->selectedRows().at(0));
    //QDir destDir;
    if (destFileInfo.exists() && destFileInfo.isDir()) {
        QDir destDir(destFileInfo.absoluteFilePath());
        for (int i = 0; i < l.size(); ++i) {
            sourceFileInfo = fileSystem->fileInfo(l.at(i));
            if (destDir.exists(sourceFileInfo.fileName()) || destDir.mkdir(sourceFileInfo.fileName())) {
                destDir.cd(sourceFileInfo.fileName());
                this->copyMP3s(sourceFileInfo.absoluteFilePath(), destDir);
                destDir.cdUp();
            }
        }
    }
}

void MyThread::run()
{
    if (toCopy)
        startCopying();
    else
        updateTotalSize();
}
