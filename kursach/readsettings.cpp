#include "ReadSettings.h"

ReadSettings::ReadSettings()
{
    setFile = new QFile("connection.txt");
    readFromFile();
    start();
}

void ReadSettings::readFromFile()
{
    if(!setFile->open(QIODevice::ReadOnly|QIODevice::Text))
        qDebug() << "error open file";
    nickName = setString(nickName);
    password = setString(password);
    email = setString(email);
    server = setString(server);
    port = setString(port);
    qDebug() << server;
    setFile->close();
}

QString ReadSettings::setString(QString list)
{

    QString temp;
    temp = setFile->readLine();
    temp.remove(temp.length() - 1, 1);
    list = temp;
    return list;

}

void ReadSettings::start()
{
    mainClient = new Client(nickName, password, email, server, port);
    mainClient->show();
}
