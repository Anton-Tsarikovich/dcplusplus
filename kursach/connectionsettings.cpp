#include "connectionsettings.h"
#include "ui_connectionsettings.h"

ConnectionSettings::ConnectionSettings(QWidget *parent) :  QWidget(parent), ui(new Ui::ConnectionSettings)
{
    ui->setupUi(this);
    setWindowTitle("Connection Settings");
    connectionFile = new QFile("connection.txt");
    textStream = new QTextStream(connectionFile);
    connect(ui->Cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->Save, SIGNAL(clicked()), this, SLOT(slotSave()));
}

ConnectionSettings::~ConnectionSettings()
{
    delete ui;
}

void ConnectionSettings::slotCancel()
{
    system("exit");
}
void ConnectionSettings::slotSave()
{
    if(!connectionFile->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "error of write file";
    }
    *textStream << ui->nickEdit->text() << endl;
    textStream->flush();
    *textStream << (ui->passwordEdit->text()) << endl;
    textStream->flush();
    *textStream << (ui->mailEdit->text()) << endl;
    textStream->flush();
    *textStream << (ui->IPEdit->text()) << endl;
    textStream->flush();
    *textStream << (ui->portEdit->text()) << endl;
    textStream->flush();
    connectionFile->close();
    system("exit");
}
