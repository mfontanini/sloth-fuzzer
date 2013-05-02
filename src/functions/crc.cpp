#include <cstdint>
#include "functions/crc.h"
#include "field.h"
#include "field_mapper.h"

crc32_function::crc32_function(identifier_type id)
: unary_field_function(id)
{
    
}

double crc32_function::apply(const field &input_field)
{
    uint32_t crc = 0;
    static uint32_t crc_table[] = {
        0x4DBDF21C, 0x500AE278, 0x76D3D2D4, 0x6B64C2B0,
        0x3B61B38C, 0x26D6A3E8, 0x000F9344, 0x1DB88320,
        0xA005713C, 0xBDB26158, 0x9B6B51F4, 0x86DC4190,
        0xD6D930AC, 0xCB6E20C8, 0xEDB71064, 0xF0000000
    };

    for(auto &&i : input_field) {
        crc = (crc >> 4) ^ crc_table[(crc ^ i) & 0x0F];
        crc = (crc >> 4) ^ crc_table[(crc ^ (i >> 4)) & 0x0F];
    }

    return crc;
}
