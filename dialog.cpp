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



hsv rgb2hsv(rgb in)
{
    hsv         out;
    double      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;

    out.v = max;                                // v
    delta = max - min;
    if( max > 0.0 ) {
        out.s = (delta / max);                  // s
    } else {
        // r = g = b = 0                        // s = 0, v is undefined
        out.s = 0.0;
        out.h = NAN;                            // its now undefined
        return out;
    }
    if( in.r >= max )                           // > is bogus, just keeps compilor happy
        out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
    else
    if( in.g >= max )
        out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if( out.h < 0.0 )
        out.h += 360.0;

    return out;
}


rgb hsv2rgb(hsv in)
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        if(isnan(in.h)) {   // in.h == NAN
            out.r = in.v;
            out.g = in.v;
            out.b = in.v;
            return out;
        }
        // error - should never happen
        out.r = 0.0;
        out.g = 0.0;
        out.b = 0.0;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i)
    {
        case 0:
            out.r = in.v;
            out.g = t;
            out.b = p;
            break;
        case 1:
            out.r = q;
            out.g = in.v;
            out.b = p;
            break;
        case 2:
            out.r = p;
            out.g = in.v;
            out.b = t;
            break;

        case 3:
            out.r = p;
            out.g = q;
            out.b = in.v;
            break;
        case 4:
            out.r = t;
            out.g = p;
            out.b = in.v;
            break;
        case 5:
        default:
            out.r = in.v;
            out.g = p;
            out.b = q;
            break;
    }
    return out;
}













/*
void Dialog::rgbToYsv(unsigned char *rgb, unsigned char *ysv)
{
    double r,g,b,S,H;
    r = rgb[0]/255;
    g = rgb[1]/255;
    b = rgb[2]/255;

    double V =MAX(r,g,b);

    double TEMP=MIN(r, g, b);
    if(V==0)
        then S=0;
    else
    S=(V-TEMP)/V;

    //Определение цветового тона/

    if(S==0)
    {

    }
    else
    {
     double Cr=(V-r)/(V-TEMP);
     double Cg=(V-g)/(V-TEMP);
     double Cb=(V-b)/(V-TEMP);

     //Цвет между желтым и пурпурным/

     if(r==V) H=Cb-Cg;

     /Цвет между голубым и желтым/

     if(g==V)  H=2+Gr-Cb;

     /Цвет между пурпурным и голубым/

     if(b==V)  H=4+Cg-Cr;
     H=H*60;

     /Приведение к положительным величинам/

     if(H<0) H=H+360;

    }

    ysv[0] = H;




}

double MAX(double a, double b, double c)
{
    if(a>b && a>c)
        return a;
    if(b>a && b>c)
        return b;
    if(c>a && c>b)
        return c;
}

double MIN(double a, double b, double c)
{
    if(a<b && a<c)
        return a;
    if(b<a && b<c)
        return b;
    if(c<a && c<b)
        return c;
}



   V=MAX(R, G, B)

   /Определение насыщенности/

   TEMP=MIN(R, G, B)
   if V=0 then S=0 else S=(V-TEMP)/V end if

   /Определение цветового тона/

   if S=0 then

    H=Hеопределенность

   else

    Cr=(V-R)/(V-TEMP)
    Cg=(V-G)/(V-TEMP)
    Cb=(V-B)/(V-TEMP)

    /Цвет между желтым и пурпурным/

    if R=V then H=Cb-Cg

    /Цвет между голубым и желтым/

    if G=V then H=2+Gr-Cb

    /Цвет между пурпурным и голубым/

    if B=V then H=4+Cg-Cr
    H=H*60

    /Приведение к положительным величинам/

    if H<0 then H=H+360 end if





/*

void Dialog::YUV2RGB(int y, int u, int v, BYTE* out)
{
  const int crv = int(1.596*65536+0.5);
  const int cgv = int(0.813*65536+0.5);
  const int cgu = int(0.391*65536+0.5);
  const int cbu = int(2.018*65536+0.5);

  int scaled_y = (y - 16) * int((255.0/219.0)*65536+0.5);

  _PixelClip PixelClip;

  out[0] = ScaledPixelClip(scaled_y + (u-128) * cbu); // blue
  out[1] = ScaledPixelClip(scaled_y - (u-128) * cgu - (v-128) * cgv); // green
  out[2] = ScaledPixelClip(scaled_y + (v-128) * crv); // red
}

void Dialog::RGB2YUV(unsigned char* rgb, unsigned char* yuv)
{


    const int cyb = rgb[2];
    const int cyg = rgb[1];
    const int cyr = rgb[0];

  _PixelClip PixelClip;

  // y can't overflow
  int y = (cyb*(rgb&255) + cyg*((rgb>>8)&255) + cyr*((rgb>>16)&255) + 0x108000) >> 16;
  int scaled_y = (y - 16) * int(255.0/219.0*65536+0.5);
  int b_y = ((rgb&255) << 16) - scaled_y;
  int u = ScaledPixelClip((b_y >> 10) * int(1/2.018*1024+0.5) + 0x800000);
  int r_y = (rgb & 0xFF0000) - scaled_y;
  int v = ScaledPixelClip((r_y >> 10) * int(1/1.596*1024+0.5) + 0x800000);
  return (y*256+u)*256+v;
}

/*
class Tweak : public GenericVideoFilter
{
public:
  Tweak( PClip _child, double _hue, double _sat, double _bright, double _cont, bool _coring, bool _sse,
         IScriptEnvironment* env );

  PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
  AVSValue __cdecl Tweak::FilterInfo(int request);

  static AVSValue __cdecl Create(AVSValue args, void* user_data, IScriptEnvironment* env);

private:
    int Sin, Cos;
    int Sat, Bright, Cont;
    bool coring, sse;

    BYTE map[256];
    int mapCos[256], mapSin[256];
};


Tweak::Tweak( PClip _child, double _hue, double _sat, double _bright, double _cont, bool _coring, bool _sse,
              IScriptEnvironment* env )
  : GenericVideoFilter(_child), coring(_coring), sse(_sse)
{
    try {   // HIDE DAMN SEH COMPILER BUG!!!
  if (vi.IsRGB())
        env->ThrowError("Tweak: YUV data only (no RGB)");
  if (vi.width % 2)
        env->ThrowError("Tweak: Width must be a multiple of 2; use Crop");

// The new "mapping" C code is faster than the iSSE code on my 3GHz P4HT - Make it optional
  if (sse && (coring || !vi.IsYUY2()))
        env->ThrowError("Tweak: SSE option only available for YUY2 with coring=false");
  if (sse && !(env->GetCPUFlags() & CPUF_INTEGER_SSE))
        env->ThrowError("Tweak: SSE option needs an iSSE capable processor");

    Sat = (int) (_sat * 512);
    Cont = (int) (_cont * 512);
    Bright = (int) _bright;

    const double Hue = (_hue * 3.14159265358979323846) / 180.0;
    const double SIN = sin(Hue);
    const double COS = cos(Hue);
    Sin = (int) (SIN * 4096 + 0.5);
    Cos = (int) (COS * 4096 + 0.5);

    int maxY = coring ? 235 : 255;
    int minY = coring ? 16 : 0;

    for (int i = 0; i < 256; i++)
    {
        // brightness and contrast
        int y = int((i - 16)*_cont + _bright + 16.5);
        map[i] = min(max(y,minY),maxY);

        // hue and saturation
        mapCos[i] = int(((i - 128) * COS * _sat + 128) * 256. + 128.5);
        mapSin[i] = int( (i - 128) * SIN * _sat        * 256. +   0.5);
    }
    }
    catch (...) { throw; }
}


PVideoFrame __stdcall Tweak::GetFrame(int n, IScriptEnvironment* env)
{
    PVideoFrame src = child->GetFrame(n, env);
    env->MakeWritable(&src);

    BYTE* srcp = src->GetWritePtr();

    int src_pitch = src->GetPitch();
    int height = src->GetHeight();
    int row_size = src->GetRowSize();

    if (sse && env->GetCPUFlags() & CPUF_INTEGER_SSE) {
        __int64 hue64 = (in64 Cos<<48) + (in64 (-Sin)<<32) + (in64 Sin<<16) + in64 Cos;
        __int64 satcont64 = (in64 Sat<<48) + (in64 Cont<<32) + (in64 Sat<<16) + in64 Cont;
        __int64 bright64 = (in64 Bright<<32) + in64 Bright;

        if (vi.IsYUY2() && (!coring)) {
            asm_tweak_ISSE_YUY2(srcp, row_size>>2, height, src_pitch-row_size, hue64, satcont64, bright64);
            return src;
        }
        else if (vi.IsYV12()) {
            //TODO: asm_tweak_ISSE_YV12 :: Maybe not ;-)
            //return src;
        }
    }

    const int maxUV = coring ? 240 : 255;
    const int minUV = coring ? 16 : 0;

    if (vi.IsYUY2()) {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < row_size; x+=4)
            {
                /* brightness and contrast
                srcp[x] = map[srcp[x]];
                srcp[x+2] = map[srcp[x+2]];

                // hue and saturation
                const int u = (mapCos[srcp[x+1]] + mapSin[srcp[x+3]]) >> 8;
                const int v = (mapCos[srcp[x+3]] - mapSin[srcp[x+1]]) >> 8;
                srcp[x+1] = min(max(u,minUV),maxUV);
                srcp[x+3] = min(max(v,minUV),maxUV);
            }
            srcp += src_pitch;
        }
    } else if (vi.IsYV12()) {
        int y;  // VC6 scoping sucks - Yes!
        for (y=0; y<height; ++y) {
            for (int x=0; x<row_size; ++x) {
                // brightness and contrast
                srcp[x] = map[srcp[x]];
            }
            srcp += src_pitch;
        }
        src_pitch = src->GetPitch(PLANAR_U);
        BYTE * srcpu = src->GetWritePtr(PLANAR_U);
        BYTE * srcpv = src->GetWritePtr(PLANAR_V);
        row_size = src->GetRowSize(PLANAR_U);
        height = src->GetHeight(PLANAR_U);
        for (y=0; y<height; ++y) {
            for (int x=0; x<row_size; ++x) {
                // hue and saturation
                const int u = (mapCos[srcpu[x]] + mapSin[srcpv[x]]) >> 8;
                const int v = (mapCos[srcpv[x]] - mapSin[srcpu[x]]) >> 8;
                srcpu[x] = min(max(u,minUV),maxUV);
                srcpv[x] = min(max(v,minUV),maxUV);
            }
            srcpu += src_pitch;
            srcpv += src_pitch;
        }
    }

    return src;
}

AVSValue __cdecl Tweak::Create(AVSValue args, void* user_data, IScriptEnvironment* env)
{
    try {   // HIDE DAMN SEH COMPILER BUG!!!
    return new Tweak(args[0].AsClip(),
                     args[1].AsFloat(0.0),      // hue
                     args[2].AsFloat(1.0),      // sat
                     args[3].AsFloat(0.0),      // bright
                     args[4].AsFloat(1.0),      // cont
                     args[5].AsBool(true),      // coring
                     args[6].AsBool(false),     // sse
                     env);
    }
    catch (...) { throw; }
}



// Integer SSE optimization by "Dividee".
void __declspec(naked) asm_tweak_ISSE_YUY2( BYTE *srcp, int w, int h, int modulo, __int64 hue,
                                       __int64 satcont, __int64 bright )
{
    static const __int64 norm = 0x0080001000800010i64;

    __asm {
        push        ebp
        push        edi
        push        esi
        push        ebx

        pxor        mm0, mm0
        movq        mm1, norm               // 128 16 128 16
        movq        mm2, [esp+16+20]        // Cos -Sin Sin Cos (fix12)
        movq        mm3, [esp+16+28]        // Sat Cont Sat Cont (fix9)
        movq        mm4, mm1
        paddw       mm4, [esp+16+36]        // 128 16+Bright 128 16+Bright

        mov         esi, [esp+16+4]         // srcp
        mov         edx, [esp+16+12]        // height
y_loop:
        mov         ecx, [esp+16+8]         // width
x_loop:
        movd        mm7, [esi]              // 0000VYUY
        punpcklbw   mm7, mm0
        psubw       mm7, mm1                //  V Y U Y
        pshufw      mm6, mm7, 0xDD          //  V U V U
        pmaddwd     mm6, mm2                // V*Cos-U*Sin V*Sin+U*Cos (fix12)
        psrad       mm6, 12                 // ? V' ? U'
        movq        mm5, mm7
        punpcklwd   mm7, mm6                // ? ? U' Y
        punpckhwd   mm5, mm6                // ? ? V' Y
        punpckldq   mm7, mm5                // V' Y U' Y
        psllw       mm7, 7                  // (fix7)
        pmulhw      mm7, mm3                // V'*Sat Y*Cont U'*Sat Y*Cont
        paddw       mm7, mm4                // V" Y" U" Y"
        packuswb    mm7, mm0                // 0000V"Y"U"Y"
        movd        [esi], mm7

        add         esi, 4
        dec         ecx
        jnz         x_loop

        add         esi, [esp+16+16]        // skip to next scanline
        dec         edx
        jnz         y_loop

        pop         ebx
        pop         esi
        pop         edi
        pop         ebp
        emms
        ret
    };
}



}







class _PixelClip {
  enum { buffer=320 };
  BYTE clip[256+buffer*2];
public:
  _PixelClip() {
    memset(clip, 0, buffer);
    for (int i=0; i<256; ++i) clip[i+buffer] = i;
    memset(clip+buffer+256, 255, buffer);
  }
  BYTE operator()(int i) { return clip[i+buffer]; }
};

static __inline BYTE ScaledPixelClip(int i) {
  return PixelClip((i+32768) >> 16);
}









*/

void Dialog::on_ButtonColourMsk_clicked()
{

}
