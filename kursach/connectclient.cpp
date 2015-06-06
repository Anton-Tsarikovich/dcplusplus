#include "connectclient.h"

ConnectClient::ConnectClient()
{

    this->passive = true;
    clientSocket = new QTcpSocket(this);
    connect(clientSocket, SIGNAL(hostFound()),this , SLOT(socketHostFound()));
    connect(clientSocket, SIGNAL(connected()), this, SLOT(socketConnected()));
    connect(clientSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
      socketStream.setDevice(clientSocket);


}
qint16 ConnectClient::reConnect(QString host, QString port)
{
  clientSocket->connectToHost(host, port.toInt());
   connect(clientSocket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
   qDebug()<< " connect to host"<<host<<" "<<port;
}
void ConnectClient::download()
{
    quint64 bytesAvailable = clientSocket->bytesAvailable();
    size += bytesAvailable;
    if (!dclist)
      /* uint bytesWritten +=*/ outFile->write(clientSocket->readAll());
    else
        dclistData.append(clientSocket->readAll());
    if (size >= totalSize)
    {
        clientSocket->close();
        if(dclist)
        {
            QByteArray decodedList;
            decodedList = WorkWithString::decodeList(dclistData);

            QTextStream* stream = new QTextStream(decodedList);
            delete stream;
         }
       else
            outFile->close();
    }
}

void ConnectClient::socketConnected()
{
    qDebug()<<"connected";
    socketStream << "$MyNick antoshka|";
    socketStream.flush();
}

void ConnectClient::socketHostFound()
{
    qDebug()<<"host found";
}

void ConnectClient::socketError(QAbstractSocket::SocketError)
{
    qDebug()<<"socket error";
}

void ConnectClient::socketReadyRead()
{
    QString data = clientSocket->readAll();
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

void ConnectClient::parseCommand(QString command)
{
    qDebug() << "CLIENT Command: " << command;
        if (command.startsWith("$"))
        {
            QStringList words = command.split(" ");
            if (words[0] == "$MyNick")
            {
                nickPassive = words[1];
            }
            else if (words[0] == "$Lock")
            {
                if(words[1].split(" ")[0].startsWith("EXTENDEDPROTOCOL"))
                    extendedClient = true;
                else
                    extendedClient = false;


                socketStream << "$Lock EXTENDEDPROTOCOLsomething Pk=AntoshkaPlusPlus|";
                socketStream.flush();
            }
            else if(words[0] == "$Supports")
            {
                if(words.contains("XmlBZList"))
                {
                    supportsXmlBZList = true;
                    qDebug() << "XmlBZList";
                    fileName = "files.xml.bz2";
                    socketStream << "$Error bz2 not avaible|";
                    clientSocket->close();
                    return;
                }
                else if(words.contains("BZList"))
                {
                    supportsBZList = true;
                    qDebug() << "BZList";
                    fileName = "MyList.bz2";
                    socketStream << "$Error bz2 not avaible|";
                    clientSocket->close();
                    return;
                }
            }
             else if (words[0] == "$Direction")
            {
            }
            else if (words[0] == "$Key")
            {
               socketStream << "$Supports BZList XmlBZList|";
               socketStream << "$Direction Download 7653|";
               socketStream << "$Key " << WorkWithString::lockToKey(words[1]) << "|";
               socketStream << "$Get " << "MyList.DcLst" <<"$1|";
               dclist = true;
               socketStream.flush();
            }
            else if (words[0] == "$FileLength")
            {

                socketStream << "$Send" << "|";
                socketStream.flush();

                size = 0;
                totalSize = words[1].toUInt();
                qDebug() << "Opening file";
               // download();

              /*  if (!dclist)
                {
                    QString trimmedFileName = m_filename;
                    trimmedFileName.replace('\\', '/');
                    int pos = trimmedFileName.lastIndexOf('/');
                    if (pos != -1)
                        trimmedFileName = m_filename.mid(pos + 1);

                    m_outFile = new QFile(m_downloadDir + QDir::separator() + trimmedFileName);

                    if(m_outFile->exists() && (m_offset == 1))
                        m_outFile->remove();

                    m_outFile->open(QIODevice::WriteOnly | QIODevice::Append);
                }*/
            }
            else if (words[0] == "$Error")
            {
                qDebug() << "Error";
                clientSocket->close();
            }
            else if (words[0] == "$MaxedOut")
            {
                qDebug() << "Maxed Out";
                clientSocket->close();
            }
            else
                qDebug() << "\"" << words[0] << "\" not understood";
        }
}

void ConnectClient::socketDisconnected()
{
    qDebug()<<"disconnected";
}
