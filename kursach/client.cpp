#include "client.h"
#include "ui_client.h"
#include <QTextCodec>
Client::Client(const QString &host, int nPort,QWidget *parent) : QMainWindow(parent), ui(new Ui::Client)
{
    codec = QTextCodec::codecForName("CP1251");
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    model = new QStandardItemModel;
    ui->setupUi(this);

    verticalHeader.append("Nick");
    model->setVerticalHeaderLabels(verticalHeader);
    connect(ui->exitAction,SIGNAL(triggered()),this,SLOT(close()));

    mainSocket = new QTcpSocket(this);
    mainSocket->connectToHost(host, nPort);
    connect(mainSocket, SIGNAL(hostFound()), SLOT(socketHostFound()));
        connect(mainSocket, SIGNAL(connected()), SLOT(socketConnected()));
    connect(mainSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(mainSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(mainSocket, SIGNAL(error(QAbstractSocket::SocketError))
            , this
            , SLOT(slotError(QAbstractSocket::SocketError)));
    socketStream.setDevice(mainSocket);
    connect(ui->enterMessageList, SIGNAL(returnPressed()), this, SLOT(slotSendToServer()));
    ui->chatView->setReadOnly(true);
}

Client::~Client()
{
    delete ui;
}

QString decodeChatMessage(QString message)
{
    QString ret = message;
    ret.replace("&#124;", "|");
    ret.replace("&#36;", "$");
    return ret;
}

void Client::slotReadyRead()
{
    QString data = mainSocket->readAll();

    buffer += data;
        if(buffer.endsWith("|"))
        {
            QStringList commands = buffer.split("|");
            foreach(QString command, commands)
            {
                parseCommand(command);
            }
            buffer = "";
        }
}

void Client::parseCommand(QString command)
{

    if(command == "")
        return;

    qDebug() << "Hub says: " << command;
    if (command.startsWith("$"))
    {
        QStringList words = command.split(" ");
        if (words[0] == "$HubName")
        {
            hubName = command.mid(9);
        }
        else if(words[0] == "$Lock")
        {
            QString lock = words[1];
            qDebug() << "Sending key:" << WorkWithString::lockToKey(lock);
            socketStream << "$Key " << WorkWithString::lockToKey(lock) << "|";
            socketStream << "$ValidateNick " << "antoshka" << "|";
            socketStream.flush();

        }
        else if (words[0] == "$Hello")
        {
            if (words[1] == "antoshka")
            {
                qDebug() << "Logged in";
                qDebug() << "Sending login info";
                socketStream << "$Version 1,0091|";
                socketStream << "$GetNickList|";
                socketStream << "$MyINFO $ALL antoshka <++ V:0.673,M:P,H:0/1/0,S:1>$ $LAN(T3)0x31$123@mail.com$0$|";
                socketStream.flush();
                getNickList();
            }
        }
        else if(words[0] == "$NickList")
                {
                    QStringList nicks = words[1].split("$$", QString::SkipEmptyParts);
                    qDebug() << "Nicks:";
                    for(int i = 0; i < nicks.size(); i++)
                    {

                        item = new QStandardItem(nicks[i]);
                        model->setItem(i - 1, 0, item);
                        ui->tableView->setModel(model);
                    }

                }
        else if (words[0].startsWith("$GetPass"))
        {
            qDebug() << "Nick requires password on this hub";
            socketStream << "$MyPass " << "9551737" << "|";
            socketStream.flush();
        }
        else if(words[0].startsWith("$LogedIn"))
        {
            qDebug() << "Successfully logged in as Op";
        }
        else if(words[0].startsWith("$BadPass"))
        {
            qDebug() << "Password wrong";
        }
        else
            qDebug() << words[0] << "not understood";

    }
    else if (command.startsWith("<"))
        {
            processChatCommand("", command, false);
        }
}
void Client::processChatCommand(QString nick, QString command, bool priv)
{
    QString message;

    QRegExp regExp("<([^>]*)>( ?)(.*)");
    if (regExp.exactMatch(command))
    {
        nick = regExp.cap(1);
        if(nick == "antoshka")
            return;
        message = decodeChatMessage(regExp.cap(3));
        ui->chatView->append(message);
    }
    else
    {
        // probably from the Hub
        message = command;
        priv = false;
    }

    qDebug() << "Got" << (priv ? "private" : "") << "message from" << nick << ":" << message;

}

void Client::getNickList()
{
    socketStream << "$GetNickList|";
    socketStream.flush();
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

}

void Client::slotConnected()
{
    ui->chatView->append("Received the connected() signal");
}
