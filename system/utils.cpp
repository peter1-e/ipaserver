#include "utils.h"
#include <sys/time.h>
#include <QPainter>
#include <QCryptographicHash>
#include <QFile>
#ifdef Q_OS_LINUX
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>
#endif

using namespace std;

Utils::Utils()
{

}

QString Utils::fileMd5(const QString &sourceFilePath)
{
    QFile sourceFile(sourceFilePath);
    qint64 fileSize = sourceFile.size();
    const qint64 bufferSize = 10240;

    if (sourceFile.open(QIODevice::ReadOnly)) {
        char buffer[bufferSize];
        int bytesRead;
        int readSize = qMin(fileSize, bufferSize);

        QCryptographicHash hash(QCryptographicHash::Md5);

        while (readSize > 0 && (bytesRead = sourceFile.read(buffer, readSize)) > 0) {
            fileSize -= bytesRead;
            hash.addData(buffer, bytesRead);
            readSize = qMin(fileSize, bufferSize);
        }

        sourceFile.close();
        return QString(hash.result().toHex());
    }
    return QString();
}

vector<string> Utils::split(const string& str, const string& delim)
{
    vector<string> res;

    if("" == str) {
        return res;
    }
    //先将要切割的字符串从string类型转换为char*类型
    char *strs = new char[str.length() + 1] ; //不要忘了
    strcpy(strs, str.c_str());

    char *d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char *p = strtok(strs, d);
    while(p) {
        string s = p; //分割获得的字符串转换为string类型
        res.push_back(s); //存入结果数组
        p = strtok(NULL, d);
    }
    return res;
}

long long Utils::get_timestamp()
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (1000 * tv.tv_sec + tv.tv_usec/1000);
}

unsigned char Utils::calcrc_1byte(unsigned char abyte)
{
   char i,crc_1byte = 0;
   for(i = 0; i < 8; i++){
       if(((crc_1byte^abyte) & 0x01)){
           crc_1byte ^= 0x18;
           crc_1byte >>= 1;
           crc_1byte |= 0x80;
       }else{
           crc_1byte>>=1;
       }
       abyte>>=1;
   }
   return (crc_1byte);
}

unsigned char Utils::crc8_verify(unsigned char *p, unsigned char len)
{
    unsigned char crc = 0;
    unsigned char *ptr = p;
    while(len--){
        //LogMI("*ptr = %X",*ptr);
        crc = calcrc_1byte(crc^*ptr++);
    }
    //LogMI("crc = %X",crc);
    return(crc);
}

char Utils::complement(char value)
{
    if(value >> 7) {
        value = ~value;
        value |= (0x01 << 7);
        value++;
    }
    return value;
}

/**
 * 字节数组转字符串
 * @param source
 *  字符数组
 * @param length
 *  字符数组长度
 * @param target
 *  字符串
 */
void Utils::uint8_2_hex(unsigned char *source, int length, unsigned char *target)
{
    unsigned char ddl, ddh;
    for (int i = 0; i < length; i++) {
        ddh = (unsigned char)(48 + source[i] / 16);
        ddl = (unsigned char)(48 + source[i] % 16);
        if (ddh > 57) ddh = (unsigned char)(ddh + (97 - 58));
        if (ddl > 57) ddl = (unsigned char)(ddl + (97 - 58));
        target[i * 2] = ddh;
        target[i * 2 + 1] = ddl;
    }
}

/**
 * 字符串转字节数组
 * @param source
 *  字符串
 * @param length
 *  字符串长度
 * @param target
 *  字节数组
 * @param n
 *  转换后长度
 */
void Utils::hex_2_uint8(const unsigned char *source, int length, unsigned char *target, int *len)
{
    size_t n = 0;
    for (int i = 0; i < length; i += 2) {
        if (source[i] >= 'a' && source[i] <= 'f') {
            target[n] = (unsigned char)(source[i] - 'a' + 10);
        } else if (source[i] >= 'A' && source[i] <= 'F') {
            target[n] = (unsigned char)(source[i] - 'A' + 10);
        } else {
            target[n] = (unsigned char)(source[i] - '0');
        }
        if (source[i + 1] >= 'a' && source[i + 1] <= 'f') {
            target[n] = (unsigned char)((target[n] << 4) | (source[i + 1] - 'a' + 10));
        } else if (source[i + 1] >= 'A' && source[i + 1] <= 'F') {
            target[n] = (unsigned char)((target[n] << 4) | (source[i + 1] - 'A' + 10));
        } else {
            target[n] = (target[n] << 4) | (source[i + 1] - '0');
        }
        ++n;
    }
    *len = n;
}

#ifdef Q_OS_LINUX
#if defined( __ARM )
#define LOCKFILE "/var/run/oven.pid"
#endif

#if not defined( __ARM ) && defined( _LINUX )
#define LOCKFILE "/var/tmp/oven.pid"
#endif

#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

int Utils::lockfile(int fd)
{
    struct flock fl;

    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    return(fcntl(fd, F_SETLK, &fl));
}

int Utils::already_running(void)
{
    int		fd;
    char	buf[16];

    fd = open(LOCKFILE, O_RDWR|O_CREAT, LOCKMODE);
    if (fd < 0) {
        syslog(LOG_ERR, "can't open %s: %s", LOCKFILE, strerror(errno));
        exit(1);
    }
    if (lockfile(fd) < 0) {
        if (errno == EACCES || errno == EAGAIN) {
            close(fd);
            return(1);
        }
        syslog(LOG_ERR, "can't lock %s: %s", LOCKFILE, strerror(errno));
        exit(1);
    }
    ftruncate(fd, 0);
    sprintf(buf, "%ld", (long)getpid());
    write(fd, buf, strlen(buf)+1);
    return(0);
}
#endif
