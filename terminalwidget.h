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
