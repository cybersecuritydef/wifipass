#ifndef WIFIPASS_H
#define WIFIPASS_H

#include <QObject>
#include <QDebug>
#include <QVector>
#include <QtXml/QDomDocument>

#include <windows.h>
#include <Wlanapi.h>

#define WLAN_PLAINTEXT_PSK 4

class WifiPass : public QObject{

    Q_OBJECT

public:
    WifiPass(QObject *parent = nullptr);
    virtual ~WifiPass(){}
    void getPass();
    QVector<QString> ssids() const;
    QVector<QString> passwds() const;
    QVector<QString> auths() const;
    QVector<QString> encrypts() const;

private:
    QVector<QString> ssid;
    QVector<QString> passwd;
    QVector<QString> auth;
    QVector<QString> encrypt;

    void parserProfileXML(LPWSTR profile);
};
#endif // WIFIPASS_H
