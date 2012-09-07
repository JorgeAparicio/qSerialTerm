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

#include "loggerwidget.h"
#include "ui_loggerwidget.h"

#include <QFileDialog>
#include <QMessageBox>

LoggerWidget::LoggerWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::LoggerWidget),
  logFile(0),
  logFileStream(0),
  elapsedTime(0),
  secondKeeper(0)
{
  ui->setupUi(this);
}

LoggerWidget::~LoggerWidget()
{
  if (logFile)
    logFile->close();

  delete ui;
}

void LoggerWidget::append(QByteArray const& text)
{
  if (logFile)
    *logFileStream << text;
}

void LoggerWidget::on_browsePushButton_clicked()
{
  ui->filePathLineEdit->setText(QFileDialog::getSaveFileName(this,
                                                             QLatin1String("Save file")));
}

void LoggerWidget::on_startPushButton_clicked()
{
  if (logFile) {
    secondKeeper->stop();

    logFile->close();

    ui->timeLabel->setText(QLatin1String("<font color=red>") +
                           elapsedTime->toString(QLatin1String("hh:mm:ss")) +
                           QLatin1String("</font>"));

    ui->filePathLineEdit->setEnabled(true);

    delete logFile;
    logFile = 0;
    delete logFileStream;
    logFileStream = 0;
    delete elapsedTime;
    elapsedTime = 0;
    delete secondKeeper;
    secondKeeper = 0;

    ui->appendCheckBox->setEnabled(true);
    ui->startPushButton->setText(QLatin1String("Start"));
  } else {
    bool ok;

    logFile = new QFile(ui->filePathLineEdit->text());

    if (ui->appendCheckBox->isChecked())
      ok = logFile->open(QIODevice::WriteOnly |
                         QIODevice::Text |
                         QIODevice::Append);
    else
      ok = logFile->open(QIODevice::WriteOnly |
                         QIODevice::Text);

    if (ok) {
      logFileStream = new QTextStream(logFile);
      elapsedTime = new QTime(0, 0, 0);
      secondKeeper = new QTimer();

      connect(secondKeeper, SIGNAL(timeout()),
              this,         SLOT(on_secondKeeper_timeout()));

      ui->timeLabel->setText(QLatin1String("<font color=green>") +
                             elapsedTime->toString(QLatin1String("hh:mm:ss")) +
                             QLatin1String("</font>"));

      ui->filePathLineEdit->setDisabled(true);

      secondKeeper->start(1000);

      ui->appendCheckBox->setDisabled(true);
      ui->startPushButton->setText(QLatin1String("Stop"));
    } else {
      delete logFile;
      logFile = 0;

      QMessageBox::warning(this, QLatin1String("Couldn't open file"),
                           QLatin1String("qSerialTerm couldn't open the "
                           "requested file, check the file path."));
    }
  }
}

void LoggerWidget::on_secondKeeper_timeout()
{
  *elapsedTime = elapsedTime->addSecs(1);

  ui->timeLabel->setText(QLatin1String("<font color=green>") +
                         elapsedTime->toString(QLatin1String("hh:mm:ss")) +
                         QLatin1String("</font>"));
}

void LoggerWidget::on_filePathLineEdit_textChanged(const QString &str)
{
  ui->startPushButton->setEnabled(str.length() != 0);
}
