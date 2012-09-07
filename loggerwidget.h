/*
 * Copyright (C) 2012 Jorge Aparicio <jorge.aparicio.r@gmail.com>
 *
 * This file is part of qSerialTerm.
 *
 * qSerialTerm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.

 * qSerialTerm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with qSerialTerm.  If not, see <http://www.gnu.org/licenses/>.
 */

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
