#include "client.h"
#include "ui_client.h"
#include <QTextCodec>
#include <QtNetwork/QNetworkInterface>
Client::Client(const QString &nick, const QString &pass, const QString &mail,
               const QString &server, const QString &port, QWidget *parent) :
                QMainWindow(parent), ui(new Ui::Client)
{
    ui->setupUi(this);
    ui->chatView->setReadOnly(true);

    nickName = nick;
    password = pass;
    email = mail;
    this->server = server;
    this->port = port;

    codec = QTextCodec::codecForName("CP1251");
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);

    usetItem = new QTableWidgetItem();
    mainSocket = new QTcpSocket(this);

    ui->userTable->setColumnCount(6);
    ui->userTable->setRowCount(0);
    ui->userTable->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Nick")));
    ui->userTable->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Share")));
    ui->userTable->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Description")));
    ui->userTable->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("Client")));
    ui->userTable->setHorizontalHeaderItem(4, new QTableWidgetItem(tr("E-Mail")));
    ui->userTable->setHorizontalHeaderItem(5, new QTableWidgetItem(tr("CID")));

    ui->userTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->userTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->userTable->setSortingEnabled(true);
    ui->userTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->userTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->userTable, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(slotContextMenu(const QPoint&)));

    sendMessageAction = new QAction("Send private message", this);
    connect(sendMessageAction, SIGNAL(triggered()), this, SLOT(slotTableClicked()));

    contextMenu = new QMenu(this);
    contextMenu->addAction(sendMessageAction);


    settingsConnection = new ConnectionSettings();

    mainSocket->connectToHost(server, port.toInt());

    connect(ui->exitAction,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->setOnActionConnectionSettings, SIGNAL(triggered()), this, SLOT(slotConnectionSettings()));
    connect(mainSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(mainSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(mainSocket, SIGNAL(error(QAbstractSocket::SocketError))
            , this
            , SLOT(slotError(QAbstractSocket::SocketError)));
    connect(ui->enterMessageList, SIGNAL(returnPressed()), this, SLOT(slotSendToServer()));
    connect(ui->userTable, SIGNAL(cellDoubleClicked(int,int)),this,SLOT(slotTableClicked(int,int)));
    socketStream.setDevice(mainSocket);

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
            setWindowTitle(command.mid(9));
        }
        else if(words[0] == "$Lock")
        {
            QString lock = words[1];
            qDebug() << "Sending key:" << WorkWithString::lockToKey(lock);
            socketStream << "$Key " << WorkWithString::lockToKey(lock) << "|";
            socketStream << "$ValidateNick " << nickName << "|";
            socketStream.flush();

        } 
        else if (words[0] == "$Hello")
        {
            if (words[1] == nickName)
            {
                qDebug() << "Logged in";
                qDebug() << "Sending login info";
                socketStream << "$Version 1,0091|";
                socketStream << "$GetNickList|";
                socketStream << "$MyINFO $ALL " << nickName <<" <AntoshkaDC++ V:0.673,M:A,H:0/1/0,S:1>$ $LAN(T3)0x32$" << email << "$0$|";
                qDebug()<< "$MyINFO $ALL " << nickName <<" <AntoshkaDC++ V:0.673,M:P,H:0/1/0,S:1>$ $LAN(T3)0x32$" << email << "$0$|"; ;
                socketStream.flush();
                getNickList();
            }
        }
        else if(words[0] == "$RevConnectToMe")
        {
            for(int i = 0; i < user.size(); i++)
            {
                if(user[i]->getName() == words[1])
                {
                    user[i]->requestConnect();
                    break;
                }
            }
        }
        else if(words[0] == "$NickList")
        {

            QStringList nicks = words[1].split("$$", QString::SkipEmptyParts);
            qDebug() << "Nicks:";
            ui->userTable->setRowCount(nicks.size());
            for(int i = 0; i < nicks.size(); i++)
            {
                ui->userTable->setItem(i,0,new QTableWidgetItem(nicks[i]));
                user.push_back(new User(nickName, nicks[i], mainSocket));
            }

        }
        else if (words[0].startsWith("$OpList"))
        {

            QStringList operators = words[1].split("$$", QString::SkipEmptyParts);
            qDebug() << "Op:";
            for(int i = 0; i < operators.size(); i++)
            {
                qDebug()<<i;
            }

        }
        else if (words[0].startsWith("$GetPass"))
        {
            qDebug() << "Nick requires password on this hub";
            socketStream << "$MyPass " << password << "|";
            socketStream.flush();
        }
        else if (words[0].startsWith("$To:"))
        {
            QString name = words[3];
            QString message;
            for(int i = 5; i < words.size(); i++)
                message.push_back(words[i] + " ");
            for(int i = 0; i < user.size(); i++)
            {
                if(user[i]->getName() == name)
                {
                    user.at(i)->getMessage(message);
                    user.at(i)->show();
                }
            }
            qDebug() << name;
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
    socketStream << "<antoshka> "<<ui->enterMessageList->text()<<"|";
    ui->enterMessageList->setText("");
    socketStream.flush();
}

void Client::slotConnected()
{
    ui->chatView->append("Received the connected() signal");
}

void Client::slotConnectionSettings()
{
    settingsConnection->show();
}
void Client::slotTableClicked()
{
    int i = ui->userTable->currentRow();
    user.at(i)->show();
    socketStream.flush();
}

void Client::slotContextMenu(const QPoint &pos)
{
    contextMenu->exec(ui->userTable->mapToGlobal(pos));
}
