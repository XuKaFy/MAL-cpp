#ifndef POINTER_H
#define POINTER_H

#include <type_traits>
#include <cassert>

#include "definition.h"

template<typename T>
class Pointer
{
public:
    template<typename U>
    friend class Pointer;

    Pointer()
        : m_pointer(nullptr), m_count(nullptr) {
    }

    Pointer(T* pointer) 
        : m_pointer(nullptr), m_count(nullptr) {
        create(pointer);
    }

    Pointer(const Pointer<T> &o) 
        : m_pointer(nullptr), m_count(nullptr) {
        copy(o);
    }

    ~Pointer() {
        release();
    }

    template<typename U>
    Pointer<U> convert() const {
        assert(m_count != nullptr);
        U* res = dynamic_cast<U*>(m_pointer);
        if(res == nullptr)
            throw Exception("Pointer<U>::convert: Cannot convert");
        return Pointer<U>(res, m_count);
    }

    const Pointer<T> & operator = (const Pointer<T> &o) {
        copy(o);
        return o;
    }

    const Pointer<T>& operator = (T* pointer) {
        create(pointer);
        return *this;
    }

    T* get() const {
        return m_pointer;
    }

    T* operator -> () const {
        return m_pointer;
    }

    operator bool () const {
        return m_count != nullptr;
    }

    int count() {
        assert(m_count != nullptr);
        return *m_count;
    }

private:
    Pointer(T* pointer, int* count)
        : m_pointer(pointer), m_count(count) {
        assert(m_count != nullptr);
        ++(*m_count);
    }

    void release() {
        if(m_count == nullptr)
            return ;
#ifdef POINTER_DEBUG
        printf("POINTER %p REMOVE --- COUNT = %d\n", m_pointer, *m_count);
#endif
        if(--(*m_count) == 0) {
#ifdef POINTER_DEBUG
            printf("POINTER %p DESTROY\n", m_pointer);
#endif
            delete m_count;
            delete m_pointer;
            m_count = nullptr;
            m_pointer = nullptr;
        }
    }

    void create(T* pointer) {
        release();
        if(pointer == nullptr)
            return ;
#ifdef POINTER_DEBUG
        printf("POINTER %p CREATE\n", pointer);
#endif
        m_pointer = pointer;
        m_count = new int(1);
    }

    void copy(const Pointer<T> &o) {
        release();
        if(o.m_pointer == nullptr)
            return ;
        m_pointer = o.m_pointer;
        m_count = o.m_count;
        ++(*m_count);
#ifdef POINTER_DEBUG
        printf("POINTER %p COPY --- COUNT = %d\n", m_pointer, *m_count);
#endif
    }

    T* m_pointer;
    int* m_count;
};

#endif // POINTER_H