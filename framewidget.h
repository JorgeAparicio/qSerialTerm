#ifndef FRAMEWIDGET_H
#define FRAMEWIDGET_H

#include <QWidget>
#include <QTimer>

#include "slider.h"

namespace Ui {
  class FrameWidget;
}

class FrameWidget : public QWidget
{
    Q_OBJECT

  public:
    enum DataType {
      uint8 = 0,
      uint16 = 1,
      uint32 = 2,
      int8 = 3,
      int16 = 4,
      int32 = 5
    };

    enum Endianness {
      Little = 0,
      Big = 1
    };

    enum Format {
      Binary = 0,
      Hexadecimal = 1
    };

    explicit FrameWidget(QWidget *parent = 0);
    ~FrameWidget();
    
  signals:
    void send(QByteArray const&);

  private slots:
    void on_dataTypeComboBox_currentIndexChanged(int);

    void on_payloadSpinBox_valueChanged(int);
    void on_minimumValueSpinBox_valueChanged(int);
    void on_maximumValueSpinBox_valueChanged(int);

    void on_continuousCheckBox_toggled(bool checked);

    void on_sendPushButton_clicked();

    void on_refreshRateTimer_timeout();

  private:
    Ui::FrameWidget *ui;
    QTimer *refreshRateTimer;
    QVector<Slider*> sliderVector;

    QByteArray encode();
    void enableSettings();
    void disableSettings();
};

#endif // FRAMEWIDGET_H
