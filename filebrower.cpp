#include "filebrower.h"
#include "global.h"

FileBrower::FileBrower(QWidget *parent) : QWidget(parent)
{
    setGeometry(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    setStyleSheet("QLabel {\
                      color: #ffffff;\
                      font: 26pt \"思源黑体 CN Regular\";\
                      background-color: #1e62a7;\
                      border: none;\
                      text-align : center;\
                    }\
                    QListWidget {\
                       color: #ffffff;\
                       font: 26pt \"思源黑体 CN Regular\";\
                       background-color: rgb(255, 255, 255,0);\
                       border: none;\
                    }\
                    QListWidget::item:selected {\
                        background-color: rgb(0, 153, 64);\
                    }");
    layout = new QVBoxLayout(this);
    layout->setMargin(0);

    lbl_title = new QLabel(this);
    lbl_title->setText(tr("Message"));
    lbl_title->setAlignment(Qt::AlignCenter);

    list_widget = new MyListWidget(this);
    list_widget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    list_widget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    layout->addWidget(lbl_title);
    layout->addWidget(list_widget);
    connect(list_widget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(showNextDirFiles(QListWidgetItem*)));
    currentDir.setPath("D:\\ipaudio\\Music");
    showCurrentDirFiles();
}

void FileBrower::showNextDirFiles(QListWidgetItem *item)
{
    ///获取鼠标双击的文件名字
    QString strName = item->text();
    //重新设置路径
    currentDir.cd(strName);
    showCurrentDirFiles();
}

void FileBrower::showCurrentDirFiles()
{
    QStringList fileList;
    fileList<<"*";
    QFileInfoList infoList = currentDir.entryInfoList(fileList,QDir::AllEntries,QDir::DirsFirst);
    //在QListWidget里显示文件列表
    this->showFileInfoList(infoList);
}

void FileBrower::showFileInfoList(QFileInfoList pInfoList)
{
    list_widget->clear();
    for(int i=0;i<pInfoList.size();i++)
    {
        QFileInfo tmpInfo = pInfoList.at(i);
        QString pFileName = tmpInfo.fileName();
        QListWidgetItem *tmpItem = new QListWidgetItem(pFileName);
        if(tmpInfo.isDir())
            tmpItem->setIcon(*getItemPropertyIcon(1));
        else
            tmpItem->setIcon(*getItemPropertyIcon(2));
        list_widget->addItem(tmpItem);
    }
}

QIcon * FileBrower::getItemPropertyIcon(int iType)
{
    QDir dir;
    QString path = dir.currentPath();
    switch(iType)
    {
    case 1:
        return new QIcon(path+"/Folder.png");
        break;
    case 2:
        return new QIcon(path+"/File.png");
        break;
    }
    return nullptr;
}
