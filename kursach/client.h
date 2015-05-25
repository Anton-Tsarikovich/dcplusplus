#ifndef CLIENT_H
#define CLIENT_H

#include "workwithstring.h"
#include "QStandardItemModel"
#include "QStandardItem"
#include "connectionsettings.h"
#include "user.h"

#include <QMainWindow>
#include <QTcpSocket>
#include <QTime>
#include <QDebug>
#include <QTextStream>
#include <QStringList>
#include <QTableWidgetItem>
#include <QAction>
#include <QVector>
#include <QMenu>

namespace Ui {
class Client;
}

class Client : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Client(const QString &nick, const QString &pass, const QString &mail,
                    const QString &server, const QString &port,QWidget *parent = 0);
    ~Client();
    
private:
    Ui::Client *ui;
    QTcpSocket *mainSocket;
    quint16     m_nNextBlockSize;
    QTextStream socketStream;
    QString buffer;
    QTextCodec *codec;
    ConnectionSettings *settingsConnection;
    QString nickName;
    QString password;
    QString email;
    QString server;
    QString port;
    QTableWidgetItem *usetItem;
    QVector<User*> user;
    QMenu *contextMenu;
    QAction *sendMessageAction;
    void parseCommand(QString command);
    void getNickList();
    void processChatCommand(QString nick, QString command, bool priv);

private slots:
    void slotError(QAbstractSocket::SocketError);
    void slotConnected();
    void slotReadyRead();
    void slotSendToServer();
    void slotConnectionSettings();
    void slotTableClicked();
    void slotContextMenu(const QPoint&);

};

#endif // CLIENT_H
