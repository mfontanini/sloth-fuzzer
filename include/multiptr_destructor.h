#ifndef FUZZER_MULTIPTR_DESTRUCTOR_H
#define FUZZER_MULTIPTR_DESTRUCTOR_H

#include <type_traits>
#include <tuple>
#include <vector>
#include <memory>

template<typename... Ts>
class multiptr_destructor {
public:
    template<typename T>
    void store_pointer(T *ptr) {
        internal_store<0>(ptr);
    }
private:
    template<typename T>
    using ptr_vector = std::vector<std::unique_ptr<T>>;
    
    template<size_t n>
    using tup_elem = typename std::tuple_element<n, std::tuple<Ts...>>::type;
    
    template<size_t n, typename T>
    using enabler = std::enable_if<std::is_base_of<tup_elem<n>, T>::value, void>;
    
    template<size_t n, typename T>
    using disabler = std::enable_if<!std::is_base_of<tup_elem<n>, T>::value, void>;
    
    template<size_t n, typename T>
    typename disabler<n, T>::type internal_store(T* ptr) {
        internal_store<n+1>(ptr);
    }
    
    template<size_t n, typename T>
    typename enabler<n, T>::type internal_store(T* ptr) {
        std::get<n>(vector_tuple).push_back(std::unique_ptr<T>(ptr));
    }
    
    
    std::tuple<ptr_vector<Ts>...> vector_tuple;
};

#endif // FUZZER_MULTIPTR_DESTRUCTOR_H
