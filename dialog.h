#ifndef DIALOG_H
#define DIALOG_H
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDialog>

namespace Ui {
class Dialog;
}

struct rgb
{
    double r;       // percent
    double g;       // percent
    double b;       // percent
};

struct hsv
{
    double h;       // angle in degrees
    double s;       // percent
    double v;       // percent
};

struct hsl
{
    double h;       // angle in degrees
    double s;       // percent
    double l;       // percent
};



class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    int gaussCoef(double sigma, double a[3], double *b0);
private slots:
    void on_pushButton_clicked();
    void testFunc();
    void on_pushButton_2_clicked();
    void setState(bool state);
    hsv rgb2hsv(rgb in);
    rgb hsv2rgb(hsv in);
    hsl rgbToHsl(rgb in);
    double hue2rgb(double p,double q,double t);
    rgb  hslToRgb(hsl in);
    QImage addColour(int value);

    void on_horizontalSlider_valueChanged(int value);

    void on_Slider_valueChanged(int value);

    void on_ButtonColourMsk_clicked();

    void on_SliderColour_valueChanged(int value);

    void on_SliderShine_valueChanged(int value);

private:
    Ui::Dialog *ui;
    QGraphicsScene* m_Scena;
    QGraphicsItem* m_GraphicsItem;
    QPixmap m_Pixmap;
    QByteArray m_array;
    QImage m_Image;
    QImage m_outImage;
    unsigned char *m_listImageIn;
    unsigned char *m_listImageOut;
    ;
};

#endif // DIALOG_H
