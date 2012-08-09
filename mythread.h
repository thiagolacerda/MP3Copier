#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QDir>
#include <QTreeView>
#include <QFileSystemModel>
#include <QLabel>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0);
    void run();
    bool toCopy;
    QTreeView *srcTree;
    QTreeView *destTree;
    QFileSystemModel *fileSystem;
    QLabel *sizeLabel;

signals:

public slots:
private:
    void startCopying();
    void updateTotalSize();

    qint64 calculateTotalSize(QDir &dir);

    void copyMP3s(QString path, QDir &dest);
};

#endif // MYTHREAD_H
