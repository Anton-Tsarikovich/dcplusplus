#ifndef CLIENT_H
#define CLIENT_H

#include "workwithstring.h"

#include <QMainWindow>
#include <QTcpSocket>
#include <QTime>
#include <QDebug>

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

private slots:
    void slotError(QAbstractSocket::SocketError);
    void slotConnected();
    void slotReadyRead();
    void slotSendToServer();
};

#endif // CLIENT_H
