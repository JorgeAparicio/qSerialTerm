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
