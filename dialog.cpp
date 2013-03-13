#include "dialog.h"
#include "ui_dialog.h"
#include <QFileDialog>
#include <QtCore/qmath.h>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),m_Pixmap(),m_Image()
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

    QRgb col;
    int red;
    int green;
    int blue;
    int width = m_Image.width();
    int height = m_Image.height();
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
               {
                   col = m_Image.pixel(i, j);
                   red = qRed(col);
                   green = qGreen(col);
                   blue = qBlue(col);
                   double vr = red;
                   double a[3];// = {red/255.0,green/255.0,blue/255.0};
                   double v = qLn(1.28415006);
                   gaussCoef((double)2.0,a,&vr);
                   //gaussCoef(0.2,a,&green);
                   //gaussCoef(0.2,a,&blue);

                    m_Image.setPixel(i, j, qRgb(red*(1-vr), green*(1-vr), blue*(1-vr)));
               }
           }

       m_Scena->removeItem(m_GraphicsItem);
       delete m_GraphicsItem;
       m_GraphicsItem = m_Scena->addPixmap(QPixmap::fromImage(m_Image));

}

void Dialog::on_pushButton_2_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,"","");
    m_Image.load(file);
    m_Pixmap = m_Pixmap.fromImage(m_Image);
    ui->graphicsView->fitInView(m_Scena->sceneRect(),Qt::IgnoreAspectRatio);
    m_GraphicsItem = m_Scena->addPixmap(m_Pixmap);

}







int Dialog::gaussCoef(double sigma, double *a, double *b0)
{
    double sigma_inv_4;

    sigma_inv_4 = sigma*sigma;

    sigma_inv_4 = 1.0/(sigma_inv_4*sigma_inv_4);

    double coef_A = sigma_inv_4*(sigma*(sigma*(sigma*1.1442707+0.0130625)-0.7500910)+0.2546730);
    double coef_W = sigma_inv_4*(sigma*(sigma*(sigma*1.3642870+0.0088755)-0.3255340)+0.3016210);
    double coef_B = sigma_inv_4*(sigma*(sigma*(sigma*1.2397166-0.0001644)-0.6363580)-0.0536068);

    double z0_abs   = qExp(coef_A);

    double z0_real  = z0_abs*qCos(coef_W);
    double z0_im    = z0_abs*qSin(coef_W);
    double z2       = qExp(coef_B);

    double z0_abs_2 = z0_abs*z0_abs;

    a[2] = 1.0/(z2*z0_abs_2);
    a[0] = (z0_abs_2+2*z0_real*z2)*a[2];
    a[1] = -(2*z0_real+z2)*a[2];

    *b0 = 1.0 - a[0] - a[1] - a[2];

    return 0;
}


/*

IplImage* kuwahara_grey(IplImage* img,int kernel_size)
{
        int nchannels= img->nChannels;
        int w=img->width;
        int h=img->height;
        IplImage* out= cvCreateImage(cvSize(w,h),IPL_DEPTH_8U,nchannels);

        int dt=kernel_size/2;

        //TODO: расширить изображение-убрать черные края.

        if(nchannels==1)  //только для одноканальных
        {
                for( int y=2*dt; y <h-2*dt ; ++y )
                {
                        uchar* ptr = (uchar*) (img->imageData + y*img->widthStep);
                        uchar* p_out = (uchar*) (out->imageData + y*out->widthStep);
                        for( int x=2*dt; x <w-2*dt; ++x )
                        {
                                 int gr=0;
                                 int sum=0;
                                 double mean1=0;
                                 double mean2=0;
                                 double mean3=0;
                                 double mean4=0;
                                 double disp1=0;
                                 double disp2=0;
                                 double disp3=0;
                                 double disp4=0;

                                 int k=0;
                                 //1
                                 sum=0;
                                 mean1=0;
                                 uchar* pd=ptr;
                                 for (int k1=0;k1<=dt;++k1)
                                 {
                                        for(int k2=-dt;k2<=0;++k2)
                                        {
                                                sum+=pd[x+k2];
                                                ++k;
                                        }
                                        pd+= img->widthStep;
                                 }
                                 mean1=sum/((dt+1)*(dt+1));

                                 pd=ptr;
                                 disp1=0;
                                 for (int k1=0;k1<=dt;++k1)
                                 {
                                        for(int k2=-dt;k2<=0;++k2)
                                        {
                                                disp1+=(pd[x+k2]-mean1)*(pd[x+k2]-mean1);
                                        }
                                        pd+= img->widthStep;
                                 }

                                 //2
                                 sum=0;
                                 mean2=0;
                                 pd=ptr+dt*img->widthStep;
                                 for (int k1=0;k1<=dt;++k1)
                                 {
                                        for(int k2=-dt;k2<=0;++k2)
                                        {
                                                sum+=pd[x+k2];
                                        }
                                        pd+= img->widthStep;
                                 }
                                 mean2=sum/((dt+1)*(dt+1));

                                 pd=ptr+dt*img->widthStep;
                                 disp2=0;
                                 for (int k1=0;k1<=dt;++k1)
                                 {
                                        for(int k2=-dt;k2<=0;++k2)
                                        {
                                                disp2+=(pd[x+k2]-mean2)*(pd[x+k2]-mean2);
                                        }
                                        pd+= img->widthStep;
                                 }

                                 //3
                                 sum=0;
                                 mean3=0;
                                 pd=ptr;
                                 for (int k1=0;k1<=dt;++k1)
                                 {
                                        for(int k2=0;k2<=dt;++k2)
                                        {
                                                sum+=pd[x+k2];
                                        }
                                        pd+= img->widthStep;
                                 }
                                 mean3=sum/((dt+1)*(dt+1));

                                 pd=ptr;
                                 disp3=0;
                                 for (int k1=0;k1<=dt;++k1)
                                 {
                                        for(int k2=0;k2<=dt;++k2)
                                        {
                                                disp3+=(pd[x+k2]-mean3)*(pd[x+k2]-mean3);
                                        }
                                        pd+= img->widthStep;
                                 }

                                 //4
                                 sum=0;
                                 mean4=0;
                                 pd=ptr+dt*img->widthStep;
                                 for (int k1=0;k1<=dt;++k1)
                                 {
                                        for(int k2=0;k2<=dt;++k2)
                                        {
                                                sum+=pd[x+k2];
                                        }
                                        pd+= img->widthStep;
                                 }
                                 mean4=sum/((dt+1)*(dt+1));

                                 pd=ptr+dt*img->widthStep;
                                 disp4=0;
                                 for (int k1=0;k1<=dt;++k1)
                                 {
                                        for(int k2=0;k2<=dt;++k2)
                                        {
                                                disp4+=(pd[x+k2]-mean4)*(pd[x+k2]-mean4);
                                        }
                                        pd+= img->widthStep;
                                 }

                                gr=0;
                                if((disp1<=disp2)&&(disp1<=disp3)&&(disp1<=disp4))
                                        gr=mean1;
                                if((disp2<=disp1)&&(disp2<=disp3)&&(disp2<=disp4))
                                        gr=mean2;
                                if((disp3<=disp1)&&(disp3<=disp2)&&(disp3<=disp4))
                                        gr=mean3;
                                if((disp4<=disp1)&&(disp4<=disp2)&&(disp4<=disp3))
                                        gr=mean4;

                                p_out[x]= gr;
                        }
                }
        }
        return out;
}
*/



void Dialog::testFunc(QImage* img,int kernel_size)
{
/*
            //int nchannels= img->alphaChannel();
            int w=img->width();
            int h=img->height();
            QImage* out= new QImage(w,h,img->format());
                    //cvCreateImage(cvSize(w,h),IPL_DEPTH_8U,nchannels);

            int dt=kernel_size/2;

            //TODO: расширить изображение-убрать черные края.

            if(nchannels==1)  //только для одноканальных
            {
                    for( int y=2*dt; y <h-2*dt ; ++y )
                    {
                            uchar* ptr = (uchar*) (img->imageData + y*img->widthStep);
                            uchar* p_out = (uchar*) (out->imageData + y*out->widthStep);
                            for( int x=2*dt; x <w-2*dt; ++x )
                            {
                                     int gr=0;
                                     int sum=0;
                                     double mean1=0;
                                     double mean2=0;
                                     double mean3=0;
                                     double mean4=0;
                                     double disp1=0;
                                     double disp2=0;
                                     double disp3=0;
                                     double disp4=0;

                                     int k=0;
                                     //1
                                     sum=0;
                                     mean1=0;
                                     uchar* pd=ptr;
                                     for (int k1=0;k1<=dt;++k1)
                                     {
                                            for(int k2=-dt;k2<=0;++k2)
                                            {
                                                    sum+=pd[x+k2];
                                                    ++k;
                                            }
                                            pd+= img->widthStep;
                                     }
                                     mean1=sum/((dt+1)*(dt+1));
                                     pd=ptr;
                                     disp1=0;
                                     for (int k1=0;k1<=dt;++k1)
                                     {
                                            for(int k2=-dt;k2<=0;++k2)
                                            {
                                                    disp1+=(pd[x+k2]-mean1)*(pd[x+k2]-mean1);
                                            }
                                            pd+= img->widthStep;
                                     }

                                     //2
                                     sum=0;
                                     mean2=0;
                                     pd=ptr+dt*img->widthStep;
                                     for (int k1=0;k1<=dt;++k1)
                                     {
                                            for(int k2=-dt;k2<=0;++k2)
                                            {
                                                    sum+=pd[x+k2];
                                            }
                                            pd+= img->widthStep;
                                     }
                                     mean2=sum/((dt+1)*(dt+1));

                                     pd=ptr+dt*img->widthStep;
                                     disp2=0;
                                     for (int k1=0;k1<=dt;++k1)
                                     {
                                            for(int k2=-dt;k2<=0;++k2)
                                            {
                                                    disp2+=(pd[x+k2]-mean2)*(pd[x+k2]-mean2);
                                            }
                                            pd+= img->widthStep;
                                     }

                                     //3
                                     sum=0;
                                     mean3=0;
                                     pd=ptr;
                                     for (int k1=0;k1<=dt;++k1)
                                     {
                                            for(int k2=0;k2<=dt;++k2)
                                            {
                                                    sum+=pd[x+k2];
                                            }
                                            pd+= img->widthStep;
                                     }
                                     mean3=sum/((dt+1)*(dt+1));

                                     pd=ptr;
                                     disp3=0;
                                     for (int k1=0;k1<=dt;++k1)
                                     {
                                            for(int k2=0;k2<=dt;++k2)
                                            {
                                                    disp3+=(pd[x+k2]-mean3)*(pd[x+k2]-mean3);
                                            }
                                            pd+= img->widthStep;
                                     }

                                     //4
                                     sum=0;
                                     mean4=0;
                                     pd=ptr+dt*img->widthStep;
                                     for (int k1=0;k1<=dt;++k1)
                                     {
                                            for(int k2=0;k2<=dt;++k2)
                                            {
                                                    sum+=pd[x+k2];
                                            }
                                            pd+= img->widthStep;
                                     }
                                     mean4=sum/((dt+1)*(dt+1));

                                     pd=ptr+dt*img->widthStep;
                                     disp4=0;
                                     for (int k1=0;k1<=dt;++k1)
                                     {
                                            for(int k2=0;k2<=dt;++k2)
                                            {
                                                    disp4+=(pd[x+k2]-mean4)*(pd[x+k2]-mean4);
                                            }
                                            pd+= img->widthStep;
                                     }

                                    gr=0;
                                    if((disp1<=disp2)&&(disp1<=disp3)&&(disp1<=disp4))
                                            gr=mean1;
                                    if((disp2<=disp1)&&(disp2<=disp3)&&(disp2<=disp4))
                                            gr=mean2;
                                    if((disp3<=disp1)&&(disp3<=disp2)&&(disp3<=disp4))
                                            gr=mean3;
                                    if((disp4<=disp1)&&(disp4<=disp2)&&(disp4<=disp3))
                                            gr=mean4;

                                    p_out[x]= gr;
                            }
        }
    }
*/
}
