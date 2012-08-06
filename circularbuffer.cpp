#include "circularbuffer.h"

CircularBuffer::CircularBuffer(size_t numberOfSamples, qreal minY, qreal maxY) :
  minY(minY),
  maxY(maxY),
  pushIndex(0),
  numberOfSamples(numberOfSamples)
{
  samples = new qreal[numberOfSamples];

  for (size_t i = 0; i < numberOfSamples; i++)
    samples[i] = 0;
}

void CircularBuffer::setMinY(qreal minY)
{
  this->minY = minY;
}

void CircularBuffer::setMaxY(qreal maxY)
{
  this->maxY = maxY;
}

void CircularBuffer::resize(size_t size)
{
  if (size == numberOfSamples)
    return;

  qreal *buffer = new qreal[size];

  if (size > numberOfSamples) {
    for (size_t i = 0; i < numberOfSamples; i++)
      buffer[i] = sample(i).y();

    for (size_t i = numberOfSamples; i < size; i++)
      buffer[i] = 0;

    pushIndex = numberOfSamples;
  } else {
    for (size_t i = 0; i < size; i++)
      buffer[i] = sample(numberOfSamples - size + i).y();

    pushIndex = 0;
  }

  delete [] samples;
  samples = buffer;

  numberOfSamples = size;
}

void CircularBuffer::pushBack(qreal const element)
{
  samples[pushIndex] = element;

  pushIndex++;

  if (pushIndex == numberOfSamples)
    pushIndex = 0;
}

QRectF CircularBuffer::boundingRect() const
{
  return QRectF(0, minY, numberOfSamples, maxY - minY);
}

size_t CircularBuffer::size() const
{
  return numberOfSamples;
}

QPointF CircularBuffer::sample(size_t const i) const
{
  int index = pushIndex + i;

  if (index >= int(numberOfSamples))
    index -= numberOfSamples;

  return QPointF(i, samples[index]);
}
