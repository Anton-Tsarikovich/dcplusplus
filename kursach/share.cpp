#include "share.h"
#include "ui_share.h"

Share::Share(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Share)
{
    ui->setupUi(this);
    model = new QDirModel;
    model->setReadOnly(false);
    ui->treeView->setModel(model);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->cancel,SIGNAL(clicked()),this, SLOT(close()));
    connect(ui->shareButton,SIGNAL(clicked()),this, SLOT(goShare()));
}

Share::~Share()
{
    delete ui;
}

void Share::goShare()
{

}
