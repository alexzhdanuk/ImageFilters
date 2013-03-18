#include "dialog.h"
#include "ui_dialog.h"
#include <QFileDialog>
#include <QtCore/qmath.h>
#include <QDebug>
#include <math.h>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),m_Pixmap(),m_Image()
{
    ui->setupUi(this);
    m_Scena = new QGraphicsScene();
    m_Scena->setSceneRect(0,0,400,400);
    ui->graphicsView->setScene(m_Scena);
    ui->Slider->setRange(0,200);
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

void Dialog::on_ButtonColourMsk_clicked()
{

    int width  = m_Image.width();
    int height = m_Image.height();
    rgb structRgb;
    hsv structHsv;
    QRgb col;

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            col = m_Image.pixel(x, y);
            structRgb.r = qRed(col);
            structRgb.g = qGreen(col);
            structRgb.b = qBlue(col);
            //m_listRGB << structRgb;
            structHsv = rgb2hsv(structRgb);
            structHsv.v=structHsv.v*ui->Slider->value()/100.0;
            if(structHsv.v>1) structHsv.v = 1;
            structRgb = hsv2rgb(structHsv);
            m_outImage.setPixel(x,y,qRgb(structRgb.r,structRgb.g,structRgb.b));
        }
    }

    m_Scena->removeItem(m_GraphicsItem);
    delete m_GraphicsItem;

    m_Scena->setSceneRect(0,0,m_outImage.width(),m_outImage.height());
    ui->graphicsView->fitInView(m_Scena->sceneRect(),Qt::KeepAspectRatio);
    m_GraphicsItem = m_Scena->addPixmap(QPixmap::fromImage(m_outImage));

}




hsv Dialog::rgb2hsv(rgb in)
{
    hsv         out;
    double      min, max, delta;
    double r,g,b;

    r = in.r/255.0, g = in.g/255.0, b = in.b/255.0;
    max = qMax(r, qMax(g, b));
    min = qMin(r, qMin(g, b));
    out.v = max;

    delta = max - min;
    out.s = max == 0 ? 0 : delta / max;

        if(max == min){
            out.h = 0; // achromatic
        }
        else
        {
            if(r>=max)
            {
                out.h = (g - b) / delta + (g < b ? 6 : 0);
            }
            if(g>=max)
            {
               out.h = (b - r) / delta + 2;
            }

            if(b>=max)
            {
                out.h = (r - g) / delta + 4;
            }

            out.h /= 6;
        }


    return out;
}


rgb Dialog::hsv2rgb(hsv in)
{
    rgb         out;

    double r, g, b;

        int i = floor(in.h * 6);
        double f = in.h * 6 - i;
        double p = in.v * (1 - in.s);
        double q = in.v * (1 - f * in.s);
        double t = in.v * (1 - (1 - f) * in.s);

        switch(i % 6){
            case 0: r = in.v, g = t, b = p; break;
            case 1: r = q, g = in.v, b = p; break;
            case 2: r = p, g = in.v, b = t; break;
            case 3: r = p, g = q, b = in.v; break;
            case 4: r = t, g = p, b = in.v; break;
            case 5: r = in.v, g = p, b = q; break;
        }

        out.r = r*255;
        out.g = g*255;
        out.b = b*255;

    return out;
}

