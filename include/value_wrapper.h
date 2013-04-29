#ifndef FUZZER_VALUE_WRAPPER_H
#define FUZZER_VALUE_WRAPPER_H

#include <iterator>
#include <memory>
#include <cstddef>
#include <type_traits>
#include "value_wrapper_impl.h"
#include "utils.h"

template<typename ValueType>
class value_wrapper_iterator;

class value_wrapper {
public:
    typedef value_wrapper_impl::value_type value_type;
    typedef value_wrapper_iterator<value_type> iterator;
    typedef value_wrapper_iterator<const value_type> const_iterator;

    value_wrapper(std::unique_ptr<value_wrapper_impl> impl);

    value_wrapper(const value_wrapper &rhs);
    value_wrapper(value_wrapper &&rhs);
    
    value_wrapper& operator=(value_wrapper rhs);
    
    // the size in bytes of this value wrapper
    size_t size() const;
    
    value_wrapper_impl &impl();
    const value_wrapper_impl &impl() const;
    
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;
    
    friend void swap(value_wrapper &lhs, value_wrapper &rhs);
    
    template<typename Impl, typename... Args>
    static value_wrapper from_impl(Args&&... args) 
    {
        return value_wrapper(make_unique<Impl>(std::forward<Args>(args)...));
    }
private:
    std::unique_ptr<value_wrapper_impl> m_impl;
};

template<typename ValueType>
class value_wrapper_iterator : 
public std::iterator<std::random_access_iterator_tag, ValueType> {
private:
    template<typename VT>
    class dereference_helper {
    public:
        typedef VT value_type;
        typedef typename std::conditional<
                    std::is_const<VT>::value, 
                    const value_wrapper_impl&,
                    value_wrapper_impl&
                >::type wrapped_type;
    
        dereference_helper(wrapped_type impl, size_t index)
        : impl(impl), index(index)
        {
            
        }
        
        typename std::enable_if<!std::is_const<VT>::value, value_type>::type 
        operator=(value_type value)
        {
            impl.set(index, value);
            return value;
        }
        
        operator value_type() const {
            return impl.get(index);
        }
    private:
        wrapped_type impl;
        size_t index;
    };
public:
    value_wrapper_iterator(value_wrapper_impl &wrapper, size_t index = 0)
    : wrapper(wrapper), index(index)
    {
        
    }
    
    value_wrapper_iterator& operator++()
    {
        index++;
        return *this;
    }
    
    value_wrapper_iterator operator++(int)
    {
        value_wrapper_iterator copied(*this);
        index++;
        return copied;
    }
    
    value_wrapper_iterator& operator--()
    {
        index--;
        return *this;
    }
    
    dereference_helper<ValueType> operator*() 
    {
        return dereference_helper<ValueType>(wrapper, index);
    }
    
    dereference_helper<ValueType> operator*() const
    {
        return dereference_helper<ValueType>(wrapper, index);
    }
    
    value_wrapper_iterator operator--(int)
    {
        value_wrapper_iterator copied(*this);
        index--;
        return copied;
    }
    
    size_t operator-(const value_wrapper_iterator& rhs) 
    {
        return index - rhs.index;
    }
    
    value_wrapper_iterator operator-(size_t offset)
    {
        return value_wrapper_iterator(wrapper, index - offset);
    }
    
    value_wrapper_iterator operator+(size_t offset) 
    {
        return value_wrapper_iterator(wrapper, index + offset);
    }
    
    bool operator==(const value_wrapper_iterator &rhs) const {
        return index == rhs.index && &wrapper.get() == &rhs.wrapper.get();
    }
    
    bool operator!=(const value_wrapper_iterator &rhs) const {
        return !(*this == rhs);
    }
private:
    std::reference_wrapper<value_wrapper_impl> wrapper;
    size_t index;
};
#endif // FUZZER_VALUE_WRAPPER_H
