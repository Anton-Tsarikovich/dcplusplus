#ifndef CLIENT_H
#define CLIENT_H

#include "workwithstring.h"

#include <QMainWindow>
#include <QTcpSocket>
#include <QTime>
#include <QDebug>
#include <QTextStream>
#include <QStringList>
#include "QStandardItemModel"
#include "QStandardItem"

namespace Ui {
class Client;
}

class Client : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Client(const QString &host,int nPort,QWidget *parent = 0);
    ~Client();
    
private:
    Ui::Client *ui;
    QTcpSocket *mainSocket;
    quint16     m_nNextBlockSize;
    QTextStream socketStream;
    QString buffer;
    QString hubName;
    QStandardItem *item;
    QStandardItemModel *model;
    QStringList verticalHeader;
    QTextCodec *codec;
    void parseCommand(QString command);
    void getNickList();
    void processChatCommand(QString nick, QString command, bool priv);

private slots:
    void slotError(QAbstractSocket::SocketError);
    void slotConnected();
    void slotReadyRead();
    void slotSendToServer();
    void socketHostFound();
    void socketConnected();

};

#endif // CLIENT_H
