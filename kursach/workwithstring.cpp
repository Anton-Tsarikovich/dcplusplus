#include "workwithstring.h"


WorkWithString::WorkWithString()
{
}

QString WorkWithString::lockToKey(QString lock)
{
    char* key = dcmakekey(lock.toAscii().data());
    QString ret = QString::fromAscii(key);
    delete(key);
    return ret;
}
char* WorkWithString::dcmakekey(const char *lock)
{
    int i, len, offset;
    char *buf, *key;
    char save;


     buf = new char[strlen(lock)];
    save = 5;
    len = 0;
    for(i = 0; lock[i]; i++) {
        buf[i] = lock[i] ^ save;
        buf[i] = ((buf[i] & 0x0F) << 4) | ((buf[i] & 0xF0) >> 4);
        save = lock[i];
        if((i != 0) && reservedchar(buf[i]))
            len += 10;
        else
            len++;
    }
    buf[0] ^= buf[i - 1];
    if(reservedchar(buf[0]))
        len += 10;
    else
        len++;

    key = (char*)malloc(len + 1);
    offset = 0;
    for(i = 0; lock[i] != 0; i++) {
        if(reservedchar(buf[i]))
            offset += sprintf(key + offset, "/%%DCN%03i%%/", buf[i]);
        else
            key[offset++] = buf[i];
    }
    key[offset] = 0;
    delete(buf);

    return(key);
}

int WorkWithString::reservedchar(unsigned char c)
{
    return((c == 0) || (c == 5) || (c == 124) || (c == 96) || (c == 126) || (c == 36));
}

QByteArray WorkWithString::decodeList(QByteArray inputData)
{
    qDebug() << "Decoding List...";

    QByteArray output;

    // These bytes must be there
    if(inputData.size() < 4 || inputData.at(0) != 'H' || inputData.at(1) != 'E' || inputData.at(2) != '3' || inputData.at(3) != 0xD)
    {
        qDebug() << "Decompression Error";
        return NULL;
    }

    // Ripped from DCLib and cross-platform-ified
    // http://dcgui.berlios.de

    int pos;
    qint32 nb_output;
    qint32 nb_couple;
    qint32 max_len = 0;
    qint32 ttl_len = 0;

    quint32 offset_pattern;
    quint32 offset_encoded;

    nb_output = ((qint32)inputData[8]) & 255;
    nb_output <<= 8;
    nb_output |= ((qint32)inputData[7]) & 255;
    nb_output <<= 8;
    nb_output |= ((qint32)inputData[6]) & 255;
    nb_output <<= 8;
    nb_output |= ((qint32)inputData[5]) & 255;

    nb_couple = inputData[9];
    nb_couple += ((((qint32)inputData[10]) & 255) << 8);

    for(pos = 0; pos < nb_couple; ++pos)
    {
        qint32 v;

        v = ((qint32)inputData[12 + pos*2]) & 255;

        if (v > max_len)
            max_len = v;

        ttl_len += v;
    }

    QMap<int, int> decode_array;

    offset_pattern=8 * (11 + nb_couple * 2);     /* position of the pattern block, it is just after the list of couples */
    offset_encoded=offset_pattern + ((ttl_len + 7) &~ 7);

    for(pos = 0; pos < nb_couple; ++pos)
    {
        quint32 v_len;
        quint32 value;

        v_len = ((qint32)inputData[12 + pos * 2]) & 255;

        value = get_bits(inputData, &offset_pattern, v_len);

        decode_array[(1 << v_len) + value] = inputData[11 + pos * 2];
    }

    int l = 0;

    while(output.size() != nb_output)
    {
        quint32 cur_val;
        quint32 nb_bit_val;

        cur_val = get_bit(inputData, &offset_encoded);
        nb_bit_val = 1;

        while(!decode_array.contains((1 << nb_bit_val) + cur_val))
        {
            cur_val = (cur_val << 1) | get_bit(inputData, &offset_encoded);
            ++nb_bit_val;
        }

        output += decode_array[(1 << nb_bit_val) + cur_val];

        ++l;
    }

    int i;
    quint8 parity = 0;

    for(i = 0; i < output.size(); ++i)
        parity ^= output[i];

    qDebug() << "Decoded List";

    return output;
}


quint32 WorkWithString::get_bit(QByteArray data, quint32* cur_pos)
{
    quint32 out;

    out = ((quint32)data[(*cur_pos)/8] >> ((*cur_pos) & 7)) & 1;

    ++(*cur_pos);

    return out;
}
quint32 WorkWithString::get_bits(QByteArray data, quint32* cur_pos, qint32 nb_bit)
{
    int i;
    quint32 res = 0;

    for(i = 0; i < nb_bit; ++i)
    {
        res = (res << 1) | get_bit(data, cur_pos);
    }

    return res;
}
