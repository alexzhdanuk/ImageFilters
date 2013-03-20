#include "dialog.h"
#include "ui_dialog.h"
#include <QFileDialog>
#include <QtCore/qmath.h>
#include <QDebug>
#include <math.h>
#include <QBuffer>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),m_Pixmap(),m_Image()
{
    ui->setupUi(this);
    m_Scena = new QGraphicsScene();
    m_Scena->setSceneRect(0,0,400,400);
    ui->graphicsView->setScene(m_Scena);
    ui->Slider->setRange(0,100);
    ui->SliderColour->setRange(0,200);
    ui->SliderShine->setRange(0,200);
    ui->horizontalSlider->setRange(1,8);
    QString str = QString::number(0)+"%";
    ui->labelSize->setText(str);
    ui->labelZost->setText(QString::number(1));
    ui->labelShine->setText(str);
    ui->labelColour->setText(str);
    ui->SliderColour->setValue(100);
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
    m_Image  = m_Image.convertToFormat(QImage::Format_ARGB32);

    m_listImageIn = m_Image.bits();
    m_listImageOut = new unsigned char[m_Image.byteCount()];


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
    hsl structHsl;
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
            structHsv.s=structHsv.s*ui->SliderColour->value()/100.0;
            if(structHsv.s>1) structHsv.s = 1;
            structHsv.v=structHsv.v*ui->SliderShine->value()/100.0;
            if(structHsv.v>1) structHsv.v = 1;
            structRgb = hsv2rgb(structHsv);
            //structHsl = rgbToHsl(structRgb);
            //structHsl.l=structHsl.l*ui->SliderShine->value()/100.0;
            //if(structHsl.l>1) structHsl.l = 1;
            //structRgb = hslToRgb(structHsl);
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



hsl Dialog::rgbToHsl(rgb in)
{
    double r,g,b;
    hsl out;

    r = in.r/255;
    g = in.g/255;
    b = in.b/255;
    double max = qMax(r, qMax(g, b));
    double min = qMax(r, qMax(g, b));
    double h, s, l = (max + min) / 2;

    if(max == min)
    {
        h = s = 0; // achromatic
    }
    else
    {
        double d = max - min;
        s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
        if(r>=max)
        {
            h = (g - b) / d + (g < b ? 6 : 0);
        }
        if(g>=max)
        {
           h = (b - r) / d + 2;
        }
        if(b>=max)
        {
             h = (r - g) / d + 4;
        }

        h /= 6;
    }
    out.h = h;
    out.l = l;
    out.s = s;

    return out;
}

double Dialog::hue2rgb(double p,double q,double t)
{
    if(t < 0) t += 1;
    if(t > 1) t -= 1;
    if(t < 1/6) return p + (q - p) * 6 * t;
    if(t < 1/2) return q;
    if(t < 2/3) return p + (q - p) * (2/3 - t) * 6;
    return p;
}


rgb  Dialog::hslToRgb(hsl in)
{
    rgb out;
    double h,s,l;
    double r, g, b;

    h=in.h;
    s=in.s;
    l=in.l;

    if(s == 0){
        r = g = b = l; // achromatic
    }
    else
    {
        double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        double p = 2 * l - q;
        r = hue2rgb(p, q, h + 1/3);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1/3);
    }
    out.r = r*255;
    out.g = g*255;
    out.b = b*255;

    return out;
}

void Dialog::on_SliderColour_valueChanged(int value)
{
    if(m_Pixmap.size() == QSize(0,0)) return;


    QString str = " %";
    ui->labelColour->setText(QString::number(value)+str);


    //m_Scena->removeItem(m_GraphicsItem);
    //delete m_GraphicsItem;

    addColour(value);

    //m_Scena->setSceneRect(0,0,m_outImage.width(),m_outImage.height());
    //ui->graphicsView->fitInView(m_Scena->sceneRect(),Qt::KeepAspectRatio);
    //m_GraphicsItem = m_Scena->addPixmap(QPixmap::fromImage(addColour(value)));

}

void Dialog::on_SliderShine_valueChanged(int value)
{
    QString str = " %";
    ui->labelShine->setText(QString::number(value)+str);
}


QImage Dialog::addColour(int value)
{
    rgb structRgb;
    hsv structHsv;

    for(int i=0; i<m_Image.byteCount();i+=4)
    {
        structRgb.b = m_listImageIn[i];
        structRgb.g = m_listImageIn[i+1];
        structRgb.r = m_listImageIn[i+2];
        structHsv = rgb2hsv(structRgb);
        structHsv.s=structHsv.s*value/100.0;
        if(structHsv.s>1) structHsv.s = 1;
        structRgb = hsv2rgb(structHsv);
        m_listImageOut[i] = structRgb.b;
        m_listImageOut[i+1] = structRgb.g;
        m_listImageOut[i+2] = structRgb.r;
        m_listImageOut[i+3] = m_listImageIn[i+3];
    }

    return  QImage((unsigned char *)m_listImageOut,m_Image.width(),m_Image.height(),m_Image.format());
}

void Dialog::on_checkSepia_clicked()
{

}

void Dialog::on_SliderContrast_valueChanged(int value)
{

}
