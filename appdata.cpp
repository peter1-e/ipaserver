#include "appdata.h"
#include <QDebug>
#include <QFile>
#include <QDate>
#include <QDateTime>
#include <QtXml>
#include <iostream>

#define CONFIG_FILE_PATH "/data/oven/config.xml"

AppData* AppData::m_appdata = nullptr;//类外初始化静态成员
static QMutex mutex;

AppData::AppData(QObject *parent) : QObject(parent)
{
    //app_data_init();
    parseConfigFile();
    connect(this, &AppData::update_qrcode, this, &AppData::set_qrcode);
}

AppData::~AppData()
{

}

AppData *AppData::getInstance()
{
    if(m_appdata == nullptr)
    {
        QMutexLocker locker(&mutex);//确保线程安全
        if(m_appdata == nullptr)
        {
            m_appdata = new AppData();
        }
    }
    return m_appdata;
}
#if 0
void AppData::load_data()
{
    parseConfigFile();
}

void AppData::store_data()
{
    saveConfigFile();
}
#endif

void AppData::load_data()
{
    QJsonParseError jsonError;

    QFile file("config.json");
    if(!file.open(QFile::ReadOnly)) {
        return;
    }
    QByteArray data = file.readAll();
    QJsonDocument doucment = QJsonDocument::fromJson(data, &jsonError);
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {
        if (doucment.isObject()) {
            QJsonObject object = doucment.object();
            if (object.contains("basic")) {
                QJsonValue value = object.value("basic");
                //parse_config_basic(&value);
            }
        }
    }
    file.close();
}

void AppData::store_data()
{
    QJsonDocument document;
    QJsonObject obj;

    //obj.insert("basic", generate_config_basic());

    document.setObject(obj);

    QFile file(QString("config.json"));
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        //qDebug()<<document.toJson();
        file.write(document.toJson());
        file.close();
    }
    system("sync");
}

void AppData::setOpenScreen(bool value)
{
    openScreen = value;
}
bool AppData::getOpenScreen()
{
    return openScreen;
}

void AppData::setAutoSleep(bool value)
{
    autoSleep = value;
}
bool AppData::getAutoSleep()
{
    return autoSleep;
}
void AppData::setLanguageId(int value)
{
    languageId = value;
}
int AppData::getLanguageId()
{
    return languageId;
}

//bool AppData::readFile(const QString &fileName)
//{
//    QFile file(fileName);
//    if (!file.open(QFile::ReadOnly | QFile::Text)) {
//        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
//                  << ": " << qPrintable(file.errorString())
//                  << std::endl;
//        return false;
//    }

//    QString errorStr;
//    int errorLine;
//    int errorColumn;

//    QDomDocument doc;
//    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
//    {
//        std::cerr << "Error: Parse error at line " << errorLine << ", "
//                  << "column " << errorColumn << ": "
//                  << qPrintable(errorStr) << std::endl;
//        return false;
//    }

//    QDomElement root = doc.documentElement();
//    if (root.tagName() != "config")
//    {
//        std::cerr << "Error: Not a config file" << std::endl;
//        return false;
//    }

//    parseRoot(root);

//    return true;
//}

bool AppData:: parseConfigFile()//读取test.config文件并获取根节点
{
    QFile file(CONFIG_FILE_PATH);
    if (!file.open(QFile::ReadOnly))
    {
        return false;
    }
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument qDomConfigFile;
    if (!qDomConfigFile.setContent(&file, false, &errorStr, &errorLine,&errorColumn))
    {
        return false;
    }
    QDomElement root = qDomConfigFile.documentElement();
    if (root.tagName() != "config")
    {
        std::cerr << "Error: Not a config file" << std::endl;
        return false;
    }
    parseRoot(root);
    file.close();
    return true;
}

void AppData::parseRoot(const QDomElement &element)
{
    qDebug()<<"parseRoot";
    QDomNode child = element.firstChild();
    while (!child.isNull())
    {
        if (child.toElement().tagName() == "Settings")
        {
            parseSettings(child.toElement());
        }
        child = child.nextSibling();
    }
}

void AppData::parseSettings(const QDomElement &element)
{
    qDebug()<<"parseSettings";
    QDomNode child = element.firstChild();
    while(!child.isNull()) {
        if(child.toElement().tagName() == "OpenScreen") {
            openScreen = (QVariant::fromValue(child.toElement().text()).toBool());
        } else if(child.toElement().tagName() == "LanguageID") {
            languageId = (child.toElement().text().toInt());
        } else if(child.toElement().tagName() == "AutoSleep") {
            autoSleep = (QVariant::fromValue(child.toElement().text()).toBool());
        } else if(child.toElement().tagName() == "openWifi") {
            openWifi = (QVariant::fromValue(child.toElement().text()).toBool());
        }
        child = child.nextSibling();
    }
}

bool AppData::saveConfigFile()
{
    qSetGlobalQHashSeed(100);//为了使 通过QDomElement保存节点属性时， 每次保存的属性的顺序固定，与qSetGlobalQHashSeed(-1);搭配着用
    //projectDoc.createProcessingInstruction("xml","version = "1.0"encoding = " UTF-8"");
    //QDomProcessingInstruction instruction = configFile.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    qDebug()<<"saveConfigFile";
    QDomDocument doc;
    QDomElement root = doc.createElement("config");
    doc.appendChild(root);
    saveRoot(doc, root);
    QFile file(CONFIG_FILE_PATH);
    if (!file.open(QFile::WriteOnly|QFile::Truncate | QFile::Text))//1.QFile::WriteOnly如果文件不存在，则创建；2.QFile::Truncate打开的同时清空
    {
        return false;
    }
    QTextStream stream( &file );
    stream.setCodec("utf-8");
    doc.save(stream,4,QDomNode::EncodingFromTextStream);
    file.close();
    qSetGlobalQHashSeed(-1);
    //system("sync");
    return true;
}


void AppData::saveSettings(QDomDocument &doc, QDomElement &element)
{
    qDebug()<<"saveSettings";
    QDomElement settings = doc.createElement("Settings");
    element.appendChild(settings);

    QDomNode openScreen = doc.createElement("OpenScreen");
    settings.appendChild(openScreen);
    openScreen.appendChild(doc.createTextNode(this->openScreen?"true":"false"));

    QDomNode LanguageID = doc.createElement("LanguageID");
    settings.appendChild(LanguageID);
    LanguageID.appendChild(doc.createTextNode(QString::number(this->languageId)));

    QDomNode autoSleep = doc.createElement("AutoSleep");
    settings.appendChild(autoSleep);
    autoSleep.appendChild(doc.createTextNode(this->autoSleep?"true":"false"));

    QDomNode openWifi = doc.createElement("openWifi");
    settings.appendChild(openWifi);
    openWifi.appendChild(doc.createTextNode(this->openWifi?"true":"false"));
}


void AppData::saveRoot(QDomDocument &doc, QDomElement &element)
{
    qDebug()<<"saveRoot";
    saveSettings(doc, element);
}

void AppData::setOpenWifi(bool value)
{
    openWifi = value;
}

bool AppData::getOpenWifi()
{
    return openWifi;
}

QString AppData::get_qrcode()
{
    return qrcode;
}

void AppData::set_qrcode(QString str)
{
    if(qrcode != str) {
        qrcode = str;
    }
}
