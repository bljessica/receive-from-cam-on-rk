#include "util.h"


int charset_convert(const char* from_charset, const char* to_charset, char* in_buf, size_t in_left, char* out_buf, size_t out_left) {
    iconv_t icd = (iconv_t)-1;
    size_t  sRet = -1;
    char  *pIn = in_buf;
    char  *pOut = out_buf;
    size_t  outLen = out_left;

    if  (NULL == from_charset || NULL == to_charset || NULL == in_buf || 0 >= in_left || NULL == out_buf || 0 >= out_left) {
        return  -1;
    }

    icd = iconv_open(to_charset, from_charset);
    if  ((iconv_t)-1 == icd) {
        return  -1;
    }

    sRet = iconv(icd, &pIn, &in_left, &pOut, &out_left);
    if  (( size_t )-1 == sRet) {
        iconv_close(icd);
        return  -1;
    }

    out_buf[outLen - out_left] = 0;
    iconv_close(icd);
    return  (int)(outLen - out_left);
}
 
int charset_convert_UTF8_TO_GB2312(char* in_buf, size_t in_left, char* out_buf, size_t out_left) {
    return charset_convert( "UTF-8", "GB2312", in_buf, in_left, out_buf, out_left);
}