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
