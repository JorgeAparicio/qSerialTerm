#ifndef LOGGERWIDGET_H
#define LOGGERWIDGET_H

#include <QWidget>

#include <QFile>
#include <QTextStream>
#include <QTime>
#include <QTimer>

namespace Ui {
  class LoggerWidget;
}

class LoggerWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit LoggerWidget(QWidget *parent = 0);
    ~LoggerWidget();

  public slots:
    void append(QByteArray const&);

  private slots:
    void on_browsePushButton_clicked();
    void on_startPushButton_clicked();

    void on_filePathLineEdit_textChanged(const QString &);

    void on_secondKeeper_timeout();

  private:
    Ui::LoggerWidget *ui;
    bool isLogging;
    QFile *logFile;
    QTextStream *logFileStream;
    QTime *elapsedTime;
    QTimer *secondKeeper;
};

#endif // LOGGERWIDGET_H
