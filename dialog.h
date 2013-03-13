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
    void testFunc(QImage* img,int kernel_size);
    void on_pushButton_2_clicked();

private:
    Ui::Dialog *ui;
    QGraphicsScene* m_Scena;
    QGraphicsItem* m_GraphicsItem;
    QPixmap m_Pixmap;
    QImage m_Image;
};

#endif // DIALOG_H
