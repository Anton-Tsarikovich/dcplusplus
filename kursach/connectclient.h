#ifndef CONNECTCLIENT_H
#define CONNECTCLIENT_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QTextStream>
#include <QObject>

#include "client.h"
#include "workwithstring.h"
class ConnectClient : public QObject
{
    Q_OBJECT
public:
    ConnectClient();
    qint16 reConnect(QString host, QString port);
    void parseCommand(QString command);
    void download();
private:
    QTcpSocket* clientSocket;
    QTcpServer* clientServer;
    QTextStream socketStream;
    QString lock;
    QString nickPassive;
    QString fileName;
    QString buffer;
    QFile *outFile;
    uint size;
    uint totalSize;
    bool passive;
    bool extendedClient;
    bool supportsBZList;
    bool supportsXmlBZList;
    bool dclist;
    QString downloadDir;
    QByteArray dclistData;

private slots:

    void socketDisconnected();
    void socketReadyRead();
    void socketHostFound();
    void socketConnected();
    void socketError(QAbstractSocket::SocketError);
};

#endif // CONNECTCLIENT_H
