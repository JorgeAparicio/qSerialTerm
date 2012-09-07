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

#ifndef HEXSTRING_H
#define HEXSTRING_H

#include <QByteArray>

class HexString
{
  public:
    static QByteArray fromRawBinary(QByteArray const& bin)
    {
      static char convert[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
                                 '9', 'A', 'B', 'C', 'D', 'E', 'F'};

      QByteArray hex;

      for (int i = 0; i < bin.size(); i++) {
        hex.append(convert[quint8(bin.at(i)) / 16]);
        hex.append(convert[quint8(bin.at(i)) % 16]);
      }

      return hex;
    }

    static QByteArray toRawBinary(QByteArray const& hex)
    {
      QByteArray bin;
      quint8 low, high;

      for (int i = 0; i < hex.size(); i += 2) {
        high = hex.at(i) > '9'? hex.at(i) - 55: hex.at(i) - 48;
        low = hex.at(i + 1) > '9'? hex.at(i + 1) - 55: hex.at(i + 1) - 48;

        bin.append(16 * high + low);
      }

      return bin;
    }

  private:
    HexString();
};

#endif // HEXSTRING_H
