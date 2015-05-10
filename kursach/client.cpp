#include "client.h"
#include "ui_client.h"
Client::Client(const QString &host, int nPort,QWidget *parent) : QMainWindow(parent), ui(new Ui::Client)
{
    ui->setupUi(this);

    connect(ui->exitAction,SIGNAL(triggered()),this,SLOT(close()));

    mainSocket = new QTcpSocket(this);
    mainSocket->connectToHost(host, nPort);

    connect(mainSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(mainSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(mainSocket, SIGNAL(error(QAbstractSocket::SocketError))
            , this
            , SLOT(slotError(QAbstractSocket::SocketError)));
    connect(ui->enterMessageList, SIGNAL(returnPressed()), this, SLOT(slotSendToServer()));
    ui->chatView->setReadOnly(true);
}

Client::~Client()
{
    delete ui;
}


void Client::slotReadyRead()
{
    QByteArray arr = mainSocket->readAll();
    QString a(arr);
    qDebug() << a;
    a = WorkWithString::findCommand(a);

    ui->chatView->append(arr);
    ui->chatView->append(a);
}
void Client::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     err == QAbstractSocket::UnknownSocketError ?
                      " Unknown error" :
                     QString(mainSocket->errorString())
                    );
    ui->chatView->append(strError);
}
void Client::slotSendToServer()
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    QString messageList(ui->enterMessageList->text());
    out << quint16(0) << messageList;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    mainSocket->write(arrBlock);
    ui->enterMessageList->setText("");
}

void Client::slotConnected()
{
    ui->chatView->append("Received the connected() signal");
}
