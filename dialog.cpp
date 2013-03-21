#include "dialog.h"
#include "ui_dialog.h"
#include <QFileDialog>
#include <QtCore/qmath.h>
#include <QDebug>
#include <math.h>
#include <QBuffer>
#include <qmessagebox.h>
#include <qapplication.h>


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
	ui->SliderSepia->setRange(0,100);
	ui->SliderColour->setValue(100);
	ui->SliderShine->setValue(100);
	
	ui->SliderBlue->setRange(0,100);
	ui->SliderGreen->setRange(0,100);
	ui->SliderRed->setRange(0,100);
	ui->SliderContrast->setRange(0,100);
	ui->SliderContrast->setValue(50);
	ui->SliderRed->setValue(50);
	ui->SliderGreen->setValue(50);
	ui->SliderBlue->setValue(50);
	ui->progressBar->setValue(0);



    ui->horizontalSlider->setRange(1,8);
    QString str = QString::number(0)+"%";
    ui->labelSize->setText(str);
    ui->labelZost->setText(QString::number(1));
    ui->labelShine->setText(str);
    ui->labelColour->setText(str);
    
	connect(ui->SliderSepia,SIGNAL(valueChanged(int)),this,SLOT(onSliderSepia(int)));

	connect(ui->SliderBlue,SIGNAL(valueChanged(int)),this,SLOT(onRgbSliders(int)));
	connect(ui->SliderGreen,SIGNAL(valueChanged(int)),this,SLOT(onRgbSliders(int)));
	connect(ui->SliderRed,SIGNAL(valueChanged(int)),this,SLOT(onRgbSliders(int)));
	connect(ui->ButtonTexture,SIGNAL(clicked()),this,SLOT(onTexture()));

}

void Dialog::setState(bool state)
{
    ui->horizontalSlider->setEnabled(state);
    ui->Slider->setEnabled(state);
    ui->pushButton_2->setEnabled(state);
}


Dialog::~Dialog()
{
    delete ui;
}

void Dialog::onTexture()
{
	QString file = QFileDialog::getOpenFileName(this,"","");
	m_Texture.load(file);
	m_Texture = m_Texture.convertToFormat(QImage::Format_ARGB32);
	m_Texture = m_Texture.scaled(m_Image.width(),m_Image.height());
	m_listTexture = m_Texture.bits();

	m_Scena->removeItem(m_GraphicsItem);
    delete m_GraphicsItem;

    m_Scena->setSceneRect(0,0,m_Image.width(),m_Image.height());
    ui->graphicsView->fitInView(m_Scena->sceneRect(),Qt::KeepAspectRatio);
	m_GraphicsItem = m_Scena->addPixmap(QPixmap::fromImage(addTexture()));
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

void Dialog::on_horizontalSlider_valueChanged(int value)
{
	
    //slider of zhost
    ui->labelZost->setText(QString::number(value));
	if(m_Pixmap.size() == QSize(0,0)) return;
    
	setState(false);
    m_Scena->removeItem(m_GraphicsItem);
    delete m_GraphicsItem;

    m_Scena->setSceneRect(0,0,m_Image.width(),m_Image.height());
    ui->graphicsView->fitInView(m_Scena->sceneRect(),Qt::KeepAspectRatio);
	m_GraphicsItem = m_Scena->addPixmap(QPixmap::fromImage(addBlur(value)));
    setState(true);
	ui->progressBar->setValue(0);
	QMessageBox::information(0,"Work","already is done!!!");
}

void Dialog::on_Slider_valueChanged(int value)
{
    QString str = QString::number(value)+"%";
    ui->labelSize->setText(str);
	if(m_Pixmap.size() == QSize(0,0)) return;
    
	setState(false);
    
	m_Scena->removeItem(m_GraphicsItem);
    delete m_GraphicsItem;

    m_Scena->setSceneRect(0,0,m_outImage.width(),m_outImage.height());
    ui->graphicsView->fitInView(m_Scena->sceneRect(),Qt::KeepAspectRatio);
	m_GraphicsItem = m_Scena->addPixmap(QPixmap::fromImage(addBlur(ui->horizontalSlider->value())));
    setState(true);

	QMessageBox::information(0,"Work","already is done!!!");
}

void Dialog::on_SliderColour_valueChanged(int value)
{
    if(m_Pixmap.size() == QSize(0,0)) return;

    QString str = " %";
    ui->labelColour->setText(QString::number(value)+str);

    m_Scena->removeItem(m_GraphicsItem);
    delete m_GraphicsItem;

    m_Scena->setSceneRect(0,0,m_Image.width(),m_Image.height());
    ui->graphicsView->fitInView(m_Scena->sceneRect(),Qt::KeepAspectRatio);
    m_GraphicsItem = m_Scena->addPixmap(QPixmap::fromImage(addColour(value)));

}

void Dialog::on_SliderShine_valueChanged(int value)
{
	if(m_Pixmap.size() == QSize(0,0)) return;

    QString str = " %";
    ui->labelShine->setText(QString::number(value)+str);
	
    m_Scena->removeItem(m_GraphicsItem);
    delete m_GraphicsItem;

    m_Scena->setSceneRect(0,0,m_Image.width(),m_Image.height());
    ui->graphicsView->fitInView(m_Scena->sceneRect(),Qt::KeepAspectRatio);
    m_GraphicsItem = m_Scena->addPixmap(QPixmap::fromImage(addShine(value)));


}

void Dialog::on_SliderContrast_valueChanged(int value)
{
	
    QString str = " %";
	ui->labelContrast->setText(QString::number(value-50)+str);
	
	if(m_Pixmap.size() == QSize(0,0)) return;
	 m_Scena->removeItem(m_GraphicsItem);
    delete m_GraphicsItem;

    m_Scena->setSceneRect(0,0,m_Image.width(),m_Image.height());
    ui->graphicsView->fitInView(m_Scena->sceneRect(),Qt::KeepAspectRatio);
	m_GraphicsItem = m_Scena->addPixmap(QPixmap::fromImage(contrastFilter(value)));
}

void Dialog::onSliderSepia(int value)
{
	
    QString str = " %";
    ui->labelSepia->setText(QString::number(value)+str);
	
	if(m_Pixmap.size() == QSize(0,0)) return;
	int optionsSepia = value==0 ? 80 : value/5+80;

    m_Scena->removeItem(m_GraphicsItem);
    delete m_GraphicsItem;

    m_Scena->setSceneRect(0,0,m_Image.width(),m_Image.height());
    ui->graphicsView->fitInView(m_Scena->sceneRect(),Qt::KeepAspectRatio);
	m_GraphicsItem = m_Scena->addPixmap(QPixmap::fromImage(sepiaEffects(optionsSepia)));
}

void Dialog::onRgbSliders(int value)
{	
	rgb options;
	options.r = ((255/100)*(ui->SliderRed->value()-50));
	options.g = ((255/100)*(ui->SliderGreen->value()-50));
	options.b = ((255/100)*(ui->SliderBlue->value()-50));
	QString str = " %";
    ui->labelBlue->setText(QString::number(ui->SliderBlue->value()-50)+str);
	ui->labelGreen->setText(QString::number(ui->SliderGreen->value()-50)+str);
	ui->labelRed->setText(QString::number(ui->SliderRed->value()-50)+str);
	
	if(m_Pixmap.size() == QSize(0,0)) return;
	
	m_Scena->removeItem(m_GraphicsItem);
    delete m_GraphicsItem;

    m_Scena->setSceneRect(0,0,m_Image.width(),m_Image.height());
    ui->graphicsView->fitInView(m_Scena->sceneRect(),Qt::KeepAspectRatio);
	m_GraphicsItem = m_Scena->addPixmap(QPixmap::fromImage(colourFilter(options)));

}

//------------------------------------------------------------------

double Dialog::Max(double a,double b,double c)
{
	double max;
	max = a;
	if(max<b) max = b;
	if(max<c) max = c;
	return max;
}

double Dialog::Min(double a,double b,double c)
{
	double min;
	min = a;
	if(min>b) min = b;
	if(min>c) min = c;
	return min;
}

int Dialog::Gray(rgb structRgb)
{
	int gray;
	gray = static_cast<int>(structRgb.r*0.30+structRgb.g*0.59+structRgb.b*0.11);
	return gray;
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

hsv Dialog::rgb2hsv(rgb in)
{
     hsv         out;
    double      min, max, delta;
    double r,g,b;

    r = in.r/255.0, g = in.g/255.0, b = in.b/255.0;
    max = Max(r, g, b);
    min = Min(r, g, b);
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
    double r,g,b,min,max;
    hsl out;

    r = in.r/255;
    g = in.g/255;
    b = in.b/255;
    max = Max(r, g, b);
    min = Min(r, g, b);
    //double h, s;
	out.l = (max + min) / 2;

    if(max == min)
    {
        out.h = out.s = 0; // achromatic
    }
    else
    {
        double d = max - min;
        if(out.l > 0.5) out.s = d / (2 - max - min);
		else out.s = d / (max + min);
		
		if(r>=max)
        {
            out.h = (g - b) / d + (g < b ? 6 : 0);
        }
        if(g>=max)
        {
           out.h = (b - r) / d + 2;
        }
        if(b>=max)
        {
             out.h = (r - g) / d + 4;
        }

        out.h /= 6;
    }
    
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

//------------------------------------------------------------------

QImage Dialog::sepiaEffects(int val)
{

	int gray;
	rgb structRgb;
	
	for(int i=0; i<m_Image.byteCount();i+=4)
    {       
			structRgb.b = m_listImageIn[i];
			structRgb.g = m_listImageIn[i+1];
			structRgb.r = m_listImageIn[i+2];
            gray = Gray(structRgb);
            gray = gray*(val/100.0);
			m_listImageOut[i] = gray*0.82;
			m_listImageOut[i+1] = gray*0.95;
			m_listImageOut[i+2] = gray;
			m_listImageOut[i+3] = m_listImageIn[i+3];
        }
	return  QImage((unsigned char *)m_listImageOut,m_Image.width(),m_Image.height(),m_Image.format());   
}

QImage Dialog::colourFilter(rgb options)
{
	rgb structRgb;

	for(int i=0; i<m_Image.byteCount();i+=4)
    {       
			structRgb.b = m_listImageIn[i];
			structRgb.g = m_listImageIn[i+1];
			structRgb.r = m_listImageIn[i+2];
            if(structRgb.r+options.r>=255) structRgb.r = 255;
            else
            {
                if(structRgb.r+options.r<0)
                {
                    structRgb.r = 0;
                    break;
                }
                structRgb.r+=options.r;
            }
			if(structRgb.g+options.g>=255) structRgb.g = 255;
               else
               {
                   if(structRgb.g+options.g<0)
                   {
                       structRgb.g = 0;
                       break;
                   }
                   structRgb.g+=options.g;
               }
               if(structRgb.b+options.b>=255) structRgb.b = 255;
               else
               {
                   if(structRgb.b+options.b<0)
                   {
                       structRgb.b = 0;
                       break;
                   }
                   structRgb.b+=options.b;
               }
			
			m_listImageOut[i] = structRgb.b;
			m_listImageOut[i+1] = structRgb.g;
			m_listImageOut[i+2] = structRgb.r;
			m_listImageOut[i+3] = m_listImageIn[i+3];
        }
	return  QImage((unsigned char *)m_listImageOut,m_Image.width(),m_Image.height(),m_Image.format());
}

QImage Dialog::contrastFilter(int value)
{
		const int L = 256;
		rgb structRgb;
        int b[L];

        int imageRows = m_Image.height();
        int imageCols = m_Image.width();

        int lAB = 0;
        
        for(int i=0; i<m_Image.byteCount();i+=4)
		{       
			structRgb.b = m_listImageIn[i];
			structRgb.g = m_listImageIn[i+1];
			structRgb.r = m_listImageIn[i+2];
        
            lAB += (int)(structRgb.r * 0.299 + structRgb.g * 0.587 + structRgb.b * 0.114);
        }

        lAB /= imageRows * imageCols;

        double k = 1.0 + value / 100.0;

        for (int i = 0; i < L; i++)
            {
                int delta = (int)i - lAB;
                int temp  = (int)(lAB + k *delta);

                if (temp < 0)
                    temp = 0;

                if (temp >= 255)
                    temp = 255;
                b[i] = (unsigned char)temp;
            }

        for(int i=0; i<m_Image.byteCount();i+=4)
		{       
			m_listImageOut[i] = b[(int)m_listImageIn[i]];
			m_listImageOut[i+1] = b[(int)m_listImageIn[i+1]];
			m_listImageOut[i+2] = b[(int)m_listImageIn[i+2]];
			m_listImageOut[i+3] = m_listImageIn[i+3];
        
         }



	return  QImage((unsigned char *)m_listImageOut,m_Image.width(),m_Image.height(),m_Image.format());
}

QImage Dialog::addTexture()
{
	
	for(int i=0; i<m_Image.byteCount();i+=4)
    { 
		m_listImageOut[i]   = m_listImageIn[i]*m_listTexture[i]/255;
		m_listImageOut[i+1] = m_listImageIn[i+1]*m_listTexture[i+1]/255;
		m_listImageOut[i+2] = m_listImageIn[i+2]*m_listTexture[i+2]/255;
		m_listImageOut[i+3] = m_listImageIn[i+3];

	}
	
	return  QImage((unsigned char *)m_listImageOut,m_Image.width(),m_Image.height(),m_Image.format());
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


QImage Dialog::addShine(int value)
{
    rgb structRgb;
    hsv structHsv;

    for(int i=0; i<m_Image.byteCount();i+=4)
    {
        //structRgb.b = m_listImageIn[i];
        //structRgb.g = m_listImageIn[i+1];
        //structRgb.r = m_listImageIn[i+2];
		structRgb.b = m_listImageOut[i];
        structRgb.g = m_listImageOut[i+1];
        structRgb.r = m_listImageOut[i+2];
		
		structHsv = rgb2hsv(structRgb);
        structHsv.v=structHsv.v*value/100.0;
        if(structHsv.v>1) structHsv.v = 1;
		structRgb = hsv2rgb(structHsv);
        m_listImageOut[i] = structRgb.b;
        m_listImageOut[i+1] = structRgb.g;
        m_listImageOut[i+2] = structRgb.r;
        m_listImageOut[i+3] = m_listImageIn[i+3];
    }

    return  QImage((unsigned char *)m_listImageOut,m_Image.width(),m_Image.height(),m_Image.format());
}

QImage Dialog::addBlur(int value)
{

	if(ui->Slider->value()==0) return m_Image;
    
    QRgb col;
    int width = m_Image.width();
    int height = m_Image.height();
    int otstWidth = ((width/2)*(ui->Slider->value()/100.0));
    int otstHeight = ((height/2)*(ui->Slider->value()/100.0));
	int r,g,b;
    double rSum = 0, gSum = 0, bSum = 0, kSum = 0;
	
	
	int kernelWidth = 1 + 2*value;
	int kernelHeight = 1 + 2*value;
	
		   
	

    setState(false);

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
           rSum = 0, gSum = 0, bSum = 0, kSum = 0;
		   
			m_listImageOut[4 * (width * y + x) + 3] = m_listImageIn[4 * (width * y + x) + 3];

            if(((x >= otstWidth && x<=width-otstWidth) && (y >= otstHeight &&  y<= height-otstHeight)) && (otstHeight != height && otstWidth != width))
            {
				m_listImageOut[4 * (width * y + x) + 0] = m_listImageIn[4 * (width * y + x) + 0];
                m_listImageOut[4 * (width * y + x) + 1] = m_listImageIn[4 * (width * y + x) + 1];
                m_listImageOut[4 * (width * y + x) + 2] = m_listImageIn[4 * (width * y + x) + 2];
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

                     b = m_listImageIn[4 * (width * pixelPosY + pixelPosX) + 0];
                     g = m_listImageIn[4 * (width * pixelPosY + pixelPosX) + 1];
                     r = m_listImageIn[4 * (width * pixelPosY + pixelPosX) + 2];


                    double kernelVal = 1.0;

                    rSum += r * kernelVal;
                    gSum += g * kernelVal;
                    bSum += b * kernelVal;

                    kSum += kernelVal;
                }
            }

            if (kSum <= 0) kSum = 1;
           
            rSum /= kSum;
            if (rSum < 0) rSum = 0;
            if (rSum > 255) rSum = 255;

            gSum /= kSum;
            if (gSum < 0) gSum = 0;
            if (gSum > 255) gSum = 255;

            bSum /= kSum;
            if (bSum < 0) bSum = 0;
            if (bSum > 255) bSum = 255;

			m_listImageOut[4 * (width * y + x) + 0] = bSum;
			m_listImageOut[4 * (width * y + x) + 1] = gSum;
			m_listImageOut[4 * (width * y + x) + 2] = rSum;
          }
    }
	return QImage((unsigned char *)m_listImageOut,m_Image.width(),m_Image.height(),m_Image.format());
}