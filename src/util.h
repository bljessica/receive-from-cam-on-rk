#include "iconv.h"
#include "stdlib.h"


int  charset_convert(const char* from_charset, const char* to_charset, char* in_buf, size_t in_left, char* out_buf, size_t out_left);
int charset_convert_UTF8_TO_GB2312(char* in_buf, size_t in_left, char* out_buf, size_t out_left);
