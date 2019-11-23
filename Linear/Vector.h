
#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class Vector
{
public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;
private:
  size_type maxSize;
  size_type bufSize;
  pointer buf;
  void changeSize()
  {
    pointer pom;
    maxSize = maxSize*2;
    pom = new value_type[maxSize];
    for(difference_type i = 0; i != static_cast<difference_type>(bufSize); ++i)
    {
      pom[i] = buf[i];
    }
    delete[] buf;
    buf = pom;
  }
public:
  Vector():maxSize(8), bufSize(0)
  {
    buf = new value_type[maxSize];
  }

  Vector(std::initializer_list<Type> l)
  {
    maxSize = l.size();
    buf = new value_type[maxSize];
    bufSize = 0;
    for (auto const &a: l)
    {
      buf[bufSize++] = a;
    }
  }

  Vector(const Vector& other)
  {
    maxSize = other.maxSize;
    bufSize = other.bufSize;
    buf = new value_type[maxSize];
    for(difference_type i = 0; i != static_cast<difference_type>(bufSize); ++i) buf[i] = other.buf[i];
  }

  Vector(Vector&& other)
  {
    maxSize = other.maxSize;
    bufSize = other.bufSize;
    buf = other.buf;
    other.buf = nullptr;
  }

  ~Vector()
  {
    delete[] buf;
  }

  Vector& operator=(const Vector& other)
  {
    if(this == &other) return *this;
    delete[] buf;
    maxSize = other.maxSize;
    bufSize = other.bufSize;
    buf = new value_type[maxSize];
    for(difference_type i = 0; i != static_cast<difference_type>(bufSize); ++i) buf[i] = other.buf[i];
    return *this;
  }

  Vector& operator=(Vector&& other)
  {
    if(this == &other) return *this;
    delete[] buf;
    maxSize = other.maxSize;
    bufSize = other.bufSize;
    buf = other.buf;
    other.maxSize = 0;
    other.bufSize = 0;
    other.buf = nullptr;
    return *this;
  }

  bool isEmpty() const
  {
    if(bufSize == 0)return true;
    return false;
  }

  size_type getSize() const
  {
    return bufSize;
  }

  void append(const Type& item)
  {
    if(maxSize == bufSize)changeSize();
    buf[bufSize] = item;
    ++bufSize;
  }

  void prepend(const Type& item)
  {
    if(maxSize == bufSize)changeSize();
    for(difference_type i = bufSize; i != 0; --i)buf[i] = buf[i-1];
    buf[0] = item;
    ++bufSize;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
    if(maxSize == bufSize)changeSize();
    ++bufSize;
    for(iterator i = end()-1; i != insertPosition; --i)*i = *(i-1);
    buf[insertPosition-cbegin()] = item;
  }

  Type popFirst()
  {
    value_type pom = buf[0];
    for(iterator i = begin(); i != end()-1; ++i)*i = *(i+1);
    --bufSize;
    return pom;
  }

  Type popLast()
  {
    value_type pom = *(cend()-1);
    --bufSize;
    return pom;
  }

  void erase(const const_iterator& possition)
  {
    for(iterator i = possition; i != end()-1; ++i)*i = *(i+1);
    --bufSize;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    difference_type pom = lastExcluded-firstIncluded;
    for(iterator i = firstIncluded; i != end()-pom; ++i)*i = *(i+pom);
    bufSize -= pom;
  }

  iterator begin()
  {
    return iterator(const_iterator(0, this));
  }

  iterator end()
  {
    return iterator(const_iterator(bufSize, this));
  }

  const_iterator cbegin() const
  {
    return const_iterator(0, this);
  }

  const_iterator cend() const
  {
    return const_iterator(bufSize, this);
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename Type>
class Vector<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Vector::value_type;
  using difference_type = typename Vector::difference_type;
  using pointer = typename Vector::const_pointer;
  using reference = typename Vector::const_reference;
private:
  size_type index;
  const Vector<value_type> *v;
public:
  explicit ConstIterator(difference_type index, const Vector<value_type> *v):index(index), v(v)
  {}
  reference operator*() const
  {
    if(index >= v->bufSize || index < 0) throw std::out_of_range(".");
    return v->buf[index];
  }

  ConstIterator& operator++()
  {
    if(index >= v->bufSize)throw std::out_of_range(".");
    ++index;
    return *this;
  }

  ConstIterator operator++(int)
  {
    ConstIterator *pom = this;
    ++(*this);
    return *pom;
  }

  ConstIterator& operator--()
  {
    if(index <= 0)throw std::out_of_range(".");
    --index;
    return *this;
  }

  ConstIterator operator--(int)
  {
    ConstIterator *pom = this;
    --(*this);
    return *pom;
  }

  ConstIterator operator+(difference_type d) const
  {
    if(index+d < 0 || index+d > v->bufSize)throw std::out_of_range(".");
    return ConstIterator(index+d, v);
  }

  ConstIterator operator-(difference_type d) const
  {
    if(index-d < 0 || index-d > v->bufSize)throw std::out_of_range(".");
    return ConstIterator(index-d, v);
  }

  bool operator==(const ConstIterator& other) const
  {
    if(index == other.index)return true;
    return false;
  }

  bool operator!=(const ConstIterator& other) const
  {
    if(index != other.index)return true;
    return false;
  }

  difference_type operator+(ConstIterator i) const
  {
    return index + i.index;
  }

  difference_type operator-(ConstIterator i) const
  {
    return index - i.index;
  }
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
  using pointer = typename Vector::pointer;
  using reference = typename Vector::reference;

  explicit Iterator()
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  Iterator operator+(difference_type d) const
  {
    return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d) const
  {
    return ConstIterator::operator-(d);
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif // AISDI_LINEAR_VECTOR_H

