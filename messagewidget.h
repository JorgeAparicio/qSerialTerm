#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>

namespace Ui {
  class MessageWidget;
}

class MessageWidget : public QWidget
{
    Q_OBJECT
    
  public:
    explicit MessageWidget(QWidget *parent = 0);
    ~MessageWidget();
    
  signals:
    void send(QByteArray);
    void echo(QByteArray);

  private slots:
    void on_asciiLineEdit_textChanged(const QString &);
    void on_byteLineEdit_textChanged(const QString &);

    void on_sendAsciiPushButton_clicked();
    void on_sendBytePushButton_clicked();

    void on_asciiLineEdit_returnPressed();
    void on_byteLineEdit_returnPressed();

  private:
    Ui::MessageWidget *ui;
};

#endif // MESSAGEWIDGET_H
