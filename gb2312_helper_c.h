#ifndef __GB2312_HELPER_H
#define __GB2312_HELPER_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __unit32_t_defined
typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;
typedef unsigned int            uint32_t;
#endif

/* all funcs ret: 1 -> ok, 0 -> err */
/* caller alloc enough memory for pout_unicodes */
extern int utf8_to_unicode(const uint8_t* putf8,const uint32_t utf8bytesize,uint32_t* pout_unicodes,uint32_t* poutlen);
extern int unicode_to_gb2312(const uint16_t unicode,uint16_t* gb2312_code);
extern int is_gb2312_cjk(const uint16_t gb2312_code);

/* if input is valid,return true strLetter is low [a-z],support ascii code */
extern int get_first_letter_gb2312(const uint16_t gb2312_code, char* strLetter);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
