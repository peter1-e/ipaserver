#ifndef FILEBROWER_H
#define FILEBROWER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QFileInfoList>
#include <QDir>
#include "mylistwidget.h"

class FileBrower : public QWidget
{
    Q_OBJECT
public:
    explicit FileBrower(QWidget *parent = nullptr);
    void showFileInfoList(QFileInfoList pInfoList);
    QIcon *getItemPropertyIcon(int iType);

signals:

public slots:
    void showCurrentDirFiles();
    void showNextDirFiles(QListWidgetItem *item);

private:
    QVBoxLayout *layout = nullptr;
    QLabel *lbl_title = nullptr;
    MyListWidget *list_widget = nullptr;
    QDir currentDir;
};

#endif // FILEBROWER_H
