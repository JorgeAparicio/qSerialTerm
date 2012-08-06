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
      UINT8 = 0,
      UINT16 = 1,
      UINT32 = 2,
      INT8 = 3,
      INT16 = 4,
      INT32 = 5
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
