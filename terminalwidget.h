#ifndef TERMINALWIDGET_H
#define TERMINALWIDGET_H

#include <QWidget>

namespace Ui {
  class TerminalWidget;
}

class TerminalWidget : public QWidget
{
    Q_OBJECT
    
  public:
    explicit TerminalWidget(QWidget *parent = 0);
    ~TerminalWidget();

  public slots:
    void display(QByteArray const&);

  private slots:
    void on_cleanPushButton_clicked();

  private:
    Ui::TerminalWidget *ui;
};

#endif // TERMINALWIDGET_H
