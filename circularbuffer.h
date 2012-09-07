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

#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <qwt_series_data.h>

class CircularBuffer : public QwtSeriesData<QPointF>
{
  public:
    CircularBuffer(size_t, qreal, qreal);

    void setMinY(qreal);
    void setMaxY(qreal);
    void resize(size_t);
    void pushBack(qreal const);

    QRectF boundingRect() const;
    size_t size() const;
    QPointF sample(size_t const) const;

  private:
    qreal minY;
    qreal maxY;
    size_t pushIndex;
    size_t numberOfSamples;
    qreal* samples;
};

#endif // CIRCULARBUFFER_H
