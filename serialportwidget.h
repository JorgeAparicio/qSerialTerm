#ifndef SERIALPORTWIDGET_H
#define SERIALPORTWIDGET_H

#include <QWidget>

#include <QtAddOnSerialPort/serialport.h>
#include <QtAddOnSerialPort/serialportinfo.h>

#include <QTimer>

namespace Ui {
  class SerialPortWidget;
}

class SerialPortWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit SerialPortWidget(QWidget *parent = 0);
    ~SerialPortWidget();

  public slots:
    void write(QByteArray);

  signals:
    void read(QByteArray);
    void communicationStart(bool);

  private slots:
    void on_getPortsPushButton_clicked();
    void on_openPortPushButton_clicked();
    void on_startCommunicationPushButton_clicked();

    void on_portComboBox_currentIndexChanged(int);
    void on_baudRateComboBox_currentIndexChanged(int);
    void on_dataBitsComboBox_currentIndexChanged(int);
    void on_stopBitsComboBox_currentIndexChanged(int);
    void on_parityComboBox_currentIndexChanged(int);
    void on_flowControlComboBox_currentIndexChanged(int);

    void on_refreshRateTimer_timeout();

  private:
    void enableCommunicationSettings();
    void disableCommunicationSettings();
    void validateCommunicationSettings();

    Ui::SerialPortWidget *ui;
    QtAddOn::SerialPort::SerialPort *serialPort;
    QList<QtAddOn::SerialPort::SerialPortInfo> serialPortInfoList;
    QTimer *refreshRateTimer;
    bool isThereCommunication;
};

#endif // SERIALPORTWIDGET_H
