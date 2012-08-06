#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>
#include <QSpacerItem>

#include "plot.h"

namespace Ui {
  class PlotWidget;
}

class PlotWidget : public QWidget
{
    Q_OBJECT
    
  public:
    enum DataType {
      UINT8 = 0,
      UINT16 = 1,
      UINT32 = 2,
      INT8 = 3,
      INT16 = 4,
      INT32 = 5,
      FLOAT = 6
    };

    enum Endianness {
      Little = 0,
      Big = 1
    };

    enum Format {
      Binary = 0,
      Hexadecimal = 1
    };

    explicit PlotWidget(QWidget *parent = 0);
    ~PlotWidget();
    
  public slots:
    void display(QByteArray);

  private slots:
    void on_goPushButton_clicked();

    void on_samplesShownSpinBox_valueChanged(int arg1);

    void updateBytesPerUpdate();

  private:
    Ui::PlotWidget *ui;
    int bytesPerUpdate;
    QVector<Plot*> plotVector;

    void decode(QByteArray);

};

#endif // PLOTWIDGET_H
