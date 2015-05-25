#ifndef USER_H
#define USER_H

#include <QWidget>
#include <QTcpSocket>
#include <QString>
#include <QTextStream>
namespace Ui {
class User;
}

class User : public QWidget
{
    Q_OBJECT
    
public:
    explicit User(QString myName, QString name, QTcpSocket *socket, QWidget *parent = 0);
    QString getName()
    {
        return name;
    }
    void getMessage(QString message);
    void requestConnect();
    void connectToClient(QString ip);
    ~User();
    
private:
    Ui::User *ui;
    QTcpSocket *tcpSocket;
    QTcpSocket *clientSocket;
    QTextStream clientStream;
    QTextStream socketStream;
    QString name;
    QString myName;
private slots:
    void sendMessage();
};

#endif // USER_H
