#pragma once
#include <inttypes.h>
#include <string>
#include <vector>

static bool utf8_to_unicode(const std::string& utf8str,std::vector<uint32_t>& unicode)
{
    const uint8_t* p = (const uint8_t*)(utf8str.c_str());
    const uint8_t* const pend = p + utf8str.size() - 1;
#define BYTE_PREFIX_NOT_10(p) (0b10 != ((*(p))>>6))
    while (p < pend) {
        if (0x80 != ((*p)&0x80)) { // 0XXXXXXX - ASCI
            unicode.push_back(*p);
            p += 1;
            continue;
        } else if (0b110 == ((*p)>>5)) { // 110XXXXX 10XXXXXX
            if ((p+1) > pend) return false;
            if (BYTE_PREFIX_NOT_10(p+1)) return false;
            uint32_t uc = ((((*p)&0x1f)<<6)|((*(p+1))&0x3f));
            unicode.push_back(uc);
            p += 2;
            continue;
        } else if (0b1110 == ((*p)>>4)) { // 1110XXXX 10XXXXXX 10XXXXXX
            if ((p+2) > pend) return false;
            if (BYTE_PREFIX_NOT_10(p+1) && BYTE_PREFIX_NOT_10(p+2)) return false;
            uint32_t uc = ((((*p)&0xf)<<12)|(((*(p+1))&0x3f)<<6)|((*(p+2))&0x3f));
            unicode.push_back(uc);
            p += 3;
            continue;
        } else if (0b11110 == ((*p)>>3)) { // 11110XXX 10XXXXXX 10XXXXXX 10XXXXXX
            if ((p+3) > pend) return false;
            if (BYTE_PREFIX_NOT_10(p+1) && BYTE_PREFIX_NOT_10(p+2) && BYTE_PREFIX_NOT_10(p+3)) return false;
            uint32_t uc = ((((*p)&0x7)<<18)|(((*(p+1))&0x3f)<<12)|(((*(p+2))&0x3f)<<6)|((*(p+3))&0x3f));
            unicode.push_back(uc);
            p += 4;
            continue;
        } else {
            return false;
        }
    }
    return true;
}
