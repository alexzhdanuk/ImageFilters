#ifndef DIALOG_H
#define DIALOG_H
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDialog>

namespace Ui {
class Dialog;
}

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

    void on_horizontalSlider_valueChanged(int value);

    void on_Slider_valueChanged(int value);

private:
    Ui::Dialog *ui;
    QGraphicsScene* m_Scena;
    QGraphicsItem* m_GraphicsItem;
    QPixmap m_Pixmap;
    QImage m_Image;
    QImage m_outImage;

};

#endif // DIALOG_H
