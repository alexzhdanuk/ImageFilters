#include "dialog.h"
#include "ui_dialog.h"
#include <QFileDialog>
#include <QtCore/qmath.h>
#include <QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),m_Pixmap(),m_Image()
{
    ui->setupUi(this);
    m_Scena = new QGraphicsScene();
    m_Scena->setSceneRect(0,0,400,400);
    ui->graphicsView->setScene(m_Scena);
    ui->Slider->setRange(0,100);
    ui->horizontalSlider->setRange(1,8);
    QString str = QString::number(0)+"%";
    ui->labelSize->setText(str);
    ui->labelZost->setText(QString::number(1));

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    if(m_Pixmap.size() == QSize(0,0)) return;
       setState(false);
       testFunc();
       m_Scena->removeItem(m_GraphicsItem);
       delete m_GraphicsItem;

       m_Scena->setSceneRect(0,0,m_outImage.width(),m_outImage.height());
       ui->graphicsView->fitInView(m_Scena->sceneRect(),Qt::KeepAspectRatio);
       m_GraphicsItem = m_Scena->addPixmap(QPixmap::fromImage(m_outImage));
       setState(true);

}

void Dialog::on_pushButton_2_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,"","");
    m_Image.load(file);
    m_outImage.load(file);
    m_Pixmap = m_Pixmap.fromImage(m_Image);
    m_Scena->setSceneRect(0,0,m_Image.width(),m_Image.height());
    ui->graphicsView->fitInView(m_Scena->sceneRect(),Qt::KeepAspectRatio);
    m_GraphicsItem = m_Scena->addPixmap(m_Pixmap);
    m_Scena->update();

}


void Dialog::testFunc()
{

    if(ui->Slider->value()==0) return;
    qDebug()<<ui->Slider->value();
    QRgb col;
    int width = m_Image.width();
    int height = m_Image.height();
    int otstWidth = ((width/2)*(ui->Slider->value()/100.0));
    int otstHeight = ((height/2)*(ui->Slider->value()/100.0));


    double kernel[17][17] = {
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};



    int kernelWidth = 1 + 2*ui->horizontalSlider->value();
    int kernelHeight = 1 + 2*ui->horizontalSlider->value();

    setState(false);

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            double rSum = 0, gSum = 0, bSum = 0, kSum = 0;


            if(((x >= otstWidth && x<=width-otstWidth) && (y >= otstHeight &&  y<= height-otstHeight)) && (otstHeight != height && otstWidth != width))
            {
                col = m_Image.pixel(x, y);
                unsigned char r = qRed(col);
                unsigned char g = qGreen(col);
                unsigned char b = qBlue(col);
                m_outImage.setPixel(x,y,qRgb(r,g,b));
                continue;
            }



            for (int i = 0; i < kernelWidth; i++)
            {
                for (int j = 0; j < kernelHeight; j++)
                {
                    int pixelPosX = x + (i - (kernelWidth / 2));
                    int pixelPosY = y + (j - (kernelHeight / 2));
                    if ((pixelPosX < 0) ||
                        (pixelPosX >= width) ||
                        (pixelPosY < 0) ||
                        (pixelPosY >= height)) continue;

                    col = m_Image.pixel(pixelPosX, pixelPosY);
                    unsigned char r = qRed(col);
                    unsigned char g = qGreen(col);
                    unsigned char b = qBlue(col);


                    double kernelVal = kernel[i][j];

                    rSum += r * kernelVal;
                    gSum += g * kernelVal;
                    bSum += b * kernelVal;

                    kSum += kernelVal;
                }
            }

            if (kSum <= 0) kSum = 1;
            //kSum = 30;
            rSum /= kSum;
            if (rSum < 0) rSum = 0;
            if (rSum > 255) rSum = 255;

            gSum /= kSum;
            if (gSum < 0) gSum = 0;
            if (gSum > 255) gSum = 255;

            bSum /= kSum;
            if (bSum < 0) bSum = 0;
            if (bSum > 255) bSum = 255;

            m_outImage.setPixel(x,y,qRgb(rSum,gSum,bSum));
          }
    }

}


void Dialog::on_horizontalSlider_valueChanged(int value)
{
    //slider of zhost
    ui->labelZost->setText(QString::number(value));

}

void Dialog::on_Slider_valueChanged(int value)
{
    QString str = QString::number(value)+"%";
    ui->labelSize->setText(str);

    //slider of oblast


}

void Dialog::setState(bool state)
{
    ui->horizontalSlider->setEnabled(state);
    ui->Slider->setEnabled(state);
    ui->pushButton->setEnabled(state);
    ui->pushButton_2->setEnabled(state);

}
