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

#ifndef PLOT_H
#define PLOT_H

#include <QWidget>

class CircularBuffer;
class QwtPlotMarker;
class QwtPlotCurve;
class QwtPlotGrid;

namespace Ui {
  class Plot;
}

class Plot : public QWidget
{
    Q_OBJECT
    
  public:
    explicit Plot(size_t, qreal, qreal, QWidget *parent = 0);
    ~Plot();
    void setId(QString const&);
    void setNumberOfSamples(size_t);
    void pushBack(qreal);
    void replot();
    
  private slots:
    void on_maxYSpinBox_valueChanged(int);
    void on_minYSpinBox_valueChanged(int);

    void on_xAxisCheckBox_toggled(bool checked);

    void on_colorComboBox_currentIndexChanged(int index);

    void on_gridCheckBox_toggled(bool checked);

  private:
    Ui::Plot *ui;
    CircularBuffer *circularBuffer;
    QwtPlotMarker *xAxis;
    QwtPlotCurve *curve;
    QwtPlotGrid *grid;
};

#endif // PLOT_H
