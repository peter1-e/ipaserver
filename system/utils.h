#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <string.h>
#include <QPixmap>

class Utils
{
public:
    Utils();
    static QString fileMd5(const QString &sourceFilePath);
    static std::vector<std::string> split(const std::string& str, const std::string& delim);
    static long long get_timestamp();
    static unsigned char calcrc_1byte(unsigned char abyte);
    static unsigned char crc8_verify(unsigned char *p, unsigned char len);

    char complement(char value);
    void uint8_2_hex(unsigned char *source, int length, unsigned char *target);
    void hex_2_uint8(const unsigned char *source, int length, unsigned char *target, int *len);
#ifdef Q_OS_LINUX
    static int already_running(void);
    static int lockfile(int fd);
#endif
};

#endif // UTILS_H
