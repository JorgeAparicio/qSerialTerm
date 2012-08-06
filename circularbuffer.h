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
