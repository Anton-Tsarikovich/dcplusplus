#include "user.h"
#include "ui_user.h"
#include <QtNetwork/QNetworkInterface>
User::User(QString myName, QString name, QTcpSocket *socket, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::User)
{
    ui->setupUi(this);
    this->name = name;
    this->myName = myName;
    setWindowTitle(name);
    tcpSocket = new QTcpSocket(this);
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->sendButton, SIGNAL(clicked()), this, SLOT(sendMessage()));
    connect(ui->chatLine, SIGNAL(returnPressed()), this, SLOT(sendMessage()));
    socketStream.setDevice(tcpSocket);
    ui->chatEdit->setReadOnly(true);
}

User::~User()
{
    delete ui;
}

void User::getMessage(QString message)
{
    ui->chatEdit->append(name + " : " + message);
}
void User::requestConnect()
{
    QString locIP;
    QList<QHostAddress> addr = QNetworkInterface::allAddresses();
    locIP = addr.first().toString();
    socketStream << "$ConnectToMe " << name << locIP << ":2323";
}

void User::sendMessage()
{
    socketStream << "$To: " << name <<" From: " << myName << " $<" << myName << "> "  << ui->chatLine->text() << "|";
    ui->chatEdit->append(myName + " : " +ui->chatLine->text());
    ui->chatLine->setText("");
    socketStream.flush();
}
void User::connectToClient(QString ip)
{
    socketStream <<"$RevConnectToMe " << myName << " " << name <<"|";
    socketStream.flush();
}
