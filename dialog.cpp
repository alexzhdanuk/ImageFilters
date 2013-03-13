#include "dialog.h"
#include "ui_dialog.h"


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    m_Scena = new QGraphicsScene();
    m_Scena->setSceneRect(0,0,400,400);
    ui->graphicsView->setScene(m_Scena);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    if(m_Pixmap.size() == QSize(0,0)) return;

    ui->graphicsView->fitInView(m_Scena->sceneRect(),Qt::KeepAspectRatio);
    QGraphicsItem* grafItem = m_Scena->addPixmap(m_Pixmap);
}
