#ifndef FUZZER_ENDIANNESS_H
#define FUZZER_ENDIANNESS_H

#include <stdint.h>

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
    #include <sys/param.h>
#endif

#if defined(__APPLE__)
    #include <sys/types.h>
    #define FUZZER_IS_LITTLE_ENDIAN (BYTE_ORDER == LITTLE_ENDIAN)
    #define FUZZER_IS_BIG_ENDIAN (BYTE_ORDER == BIG_ENDIAN)
#elif defined(BSD)
    #include <sys/endian.h>
    #define FUZZER_IS_LITTLE_ENDIAN (_BYTE_ORDER == _LITTLE_ENDIAN)
    #define FUZZER_IS_BIG_ENDIAN (_BYTE_ORDER == _BIG_ENDIAN)
#elif defined(WIN32)
    // Assume windows == little endian. fixme later
    #define FUZZER_IS_LITTLE_ENDIAN 1
    #define FUZZER_IS_BIG_ENDIAN 0
#else
    #include <endian.h>
    #define FUZZER_IS_LITTLE_ENDIAN (__BYTE_ORDER == __LITTLE_ENDIAN)
    #define FUZZER_IS_BIG_ENDIAN (__BYTE_ORDER == __BIG_ENDIAN)
#endif



namespace Endian {

    inline uint16_t do_change_endian(uint16_t data) {
        return ((data & 0xff00) >> 8)  | ((data & 0x00ff) << 8);
    }

    inline uint32_t do_change_endian(uint32_t data) {
        return (((data & 0xff000000) >> 24) | ((data & 0x00ff0000) >> 8)  |
                ((data & 0x0000ff00) << 8)  | ((data & 0x000000ff) << 24));
    }
    
    inline uint64_t do_change_endian(uint64_t data) {
        return (((uint64_t)(do_change_endian((uint32_t)((data << 32) >> 32))) << 32) |
            (do_change_endian(((uint32_t)(data >> 32)))));
    }
    
    // Helpers to convert
    template<typename T>
    struct conversion_dispatch_helper {
        static T dispatch(T data) {
            return do_change_endian(data);
        }
    };
    
    
    template<size_t>
    struct conversion_dispatcher;
    
    template<>
    struct conversion_dispatcher<sizeof(uint16_t)> 
    : public conversion_dispatch_helper<uint16_t> 
    { };
    
    template<>
    struct conversion_dispatcher<sizeof(uint32_t)> 
    : public conversion_dispatch_helper<uint32_t> 
    { };
    
    template<>
    struct conversion_dispatcher<sizeof(uint64_t)> 
    : public conversion_dispatch_helper<uint64_t> 
    { };

     template<typename T>
     inline T change_endian(T data) {
         return conversion_dispatcher<sizeof(T)>::dispatch(data);
     }
    
    #if FUZZER_IS_LITTLE_ENDIAN
        template<typename T>
        inline T host_to_be(T data) {
            return change_endian(data);
        }
         
         template<typename T>
         inline T host_to_le(T data) {
             return data;
         }
         
         template<typename T>
         inline T be_to_host(T data) {
             return change_endian(data);
         }
         
         template<typename T>
         inline T le_to_host(T data) {
             return data;
         }
    #elif FUZZER_IS_BIG_ENDIAN
        template<typename T>
        inline T host_to_be(T data) {
            return data;
        }

         template<typename T>
         inline T host_to_le(T data) {
             return change_endian(data);
         }
         
         template<typename T>
         inline T be_to_host(T data) {
             return data;
         }
         
         template<typename T>
         inline T le_to_host(T data) {
             return change_endian(data);
         }
    #endif
} // namespace Endian

#endif // FUZZER_ENDIANNESS_H
