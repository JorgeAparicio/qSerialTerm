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
