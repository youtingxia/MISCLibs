awk '
BEGIN {print "#include<inttypes.h>\n#include<unordered_map>\nstatic const std::unordered_map<uint32_t,uint16_t> s_unicode_to_gb2312_tables = \{"}
{if((NR>141)&&(NR<7587)){print "\{0x" substr($1,3,4) ",0x" substr($2,3,2) substr($2,7,2) "\},"}}
END {print "};"}' GB2312.txt > gb2312_tables.cpp

// <U3001>     /xa1/xa2     IDEOGRAPHIC COMMA
1）from line:142 to line: 7586
2) map<uint32_t/*<0x3001*/,uint32_t<0xa1a1>>
