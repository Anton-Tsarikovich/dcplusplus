#ifndef READSETTINGS_H
#define READSETTINGS_H

#include <QFile>
#include <QString>
#include "client.h"

class ReadSettings
{
public:
    ReadSettings();
    void readFromFile();

private:
    QFile *setFile;
    QString nickName;
    QString password;
    QString email;
    QString server;
    QString port;
    Client* mainClient;
    QString setString(QString list);
    void start();
};

#endif // READSETTINGS_H
