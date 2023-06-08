#ifndef APPDATA_H
#define APPDATA_H

#include <QObject>
#include <QMutex>
#include <QDomElement>
#include <QTime>

class AppData : public QObject
{
    Q_OBJECT
public:
public:
    static AppData* getInstance();
    ~AppData();
    bool parseConfigFile();
    bool saveConfigFile();
    //bool readFile(const QString &fileName);
    QString get_qrcode();

signals:
    void update_qrcode(QString);

public slots:
    void load_data();
    void store_data();
    void setOpenScreen(bool);
    bool getOpenScreen();
    void setAutoSleep(bool);
    bool getAutoSleep();
    void setLanguageId(int);
    int getLanguageId();
    void setOpenWifi(bool);
    bool getOpenWifi();
    void set_qrcode(QString str);

private:
    explicit AppData(QObject *parent = nullptr);
    void parseRoot(const QDomElement &element);
    void parseSettings(const QDomElement &element);

    void saveRoot(QDomDocument &doc, QDomElement &element);
    void saveSettings(QDomDocument &doc, QDomElement &element);
private:
    bool openScreen = true;
    bool autoSleep = true;
    int languageId = 1;
    bool openWifi = true;    //wifi开关
    QString qrcode;
    static AppData* m_appdata;
    Q_DISABLE_COPY(AppData)
};

#endif // APPDATA_H
