#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class LinkedList
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
  struct Node
  {
    pointer data;
    Node *next;
    Node *prev;
    Node():data(nullptr), next(nullptr), prev(nullptr)
    {}
    Node(const value_type data):next(nullptr), prev(nullptr)
    {
      this->data = new value_type(data);
    }
    ~Node()
    {
      if(data != nullptr)delete data;
    }
  };
  size_type listSize;
  Node *root;
  Node *last;
public:
  LinkedList():listSize(0)
  {
    root = new Node;
    last = root;
  }

  LinkedList(std::initializer_list<Type> l)
  {
    last = new Node;
    root = last;
    listSize = 0;
    for (auto const &a: l)append(a);
  }

  LinkedList(const LinkedList& other)
  {
    root = new Node;
    last = root;
    listSize = 0;
    for (auto const &a: other)append(a);
  }

  LinkedList(LinkedList&& other)
  {
    listSize = other.listSize;
    root = other.root;
    last = other.last;
    other.root = nullptr;
    other.last = nullptr;
  }

  ~LinkedList()
  {
    for(Node *i=root; i!=nullptr; i=i->next)delete i;
  }

  LinkedList& operator=(const LinkedList& other)
  {
    if(this == &other)return *this;
    for(Node *i=root; i!=nullptr; i=i->next)delete i;
    root = new Node;
    last = root;
    listSize = 0;
    for (auto const &a: other)append(a);
    return *this;
  }

  LinkedList& operator=(LinkedList&& other)
  {
    if(this == &other) return *this;
    for(Node *i=root; i!=nullptr; i=i->next)delete i;
    root = other.root;
    last = other.last;
    listSize = other.listSize;
    other.root = nullptr;
    other.last = nullptr;
    return *this;
  }

  bool isEmpty() const
  {
    if(listSize == 0)return true;
    return false;
  }

  size_type getSize() const
  {
    return listSize;
  }

  void append(const Type& item)
  {
    Node *newLast = new Node(item);
    if(listSize == 0)
    {
      root = newLast;
      root->next = last;
      last->prev = root;
    }
    else
    {
      last->prev->next = newLast;
      newLast->next = last;
      newLast->prev = last->prev;
      last->prev = newLast;
    }
    ++listSize;
  }

  void prepend(const Type& item)
  {
    Node *newRoot = new Node(item);
    ++listSize;
    if(listSize == 0)
    {
      root = newRoot;
      root->next = last;
      last->prev = root;
      return;
    }
    root->prev = newRoot;
    newRoot->next = root;
    root = root->prev;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
    if(insertPosition.current == root)prepend(item);
    else if(insertPosition.current == last)append(item);
    else
    {
      Node *newElement = new Node(item);
      newElement->next = insertPosition.current;
      newElement->prev = insertPosition.current->prev;
      insertPosition.current->prev->next = newElement;
      insertPosition.current->prev = newElement;
      listSize++;
    }
  }

  Type popFirst()
  {
    if(listSize == 0)throw std::logic_error(".");
    Node *toPop = root;
    value_type toPopData = *root->data;
    root = root->next;
    root->prev = nullptr;
    delete toPop;
    --listSize;
    return toPopData;
  }

  Type popLast()
  {
    if(listSize == 0)throw std::logic_error(".");
    Node *toPop = last->prev;
    if(listSize == 1)root = root->next;
    value_type toPopData = *last->prev->data;
    if(toPop->prev != nullptr)toPop->prev->next = toPop->next;
    toPop->next->prev = toPop->prev;
    delete toPop;
    --listSize;
    return toPopData;
  }

  void erase(const const_iterator& possition)
  {
    *possition;
    possition.current->next->prev = possition.current->prev;
    if(possition.current->prev!=nullptr)possition.current->prev->next = possition.current->next;
    else root = root->next;
    listSize--;
    delete possition.current;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    for(iterator i = firstIncluded; i != lastExcluded; ++i)erase(i);
  }

  iterator begin()
  {
    return iterator(const_iterator(root, this));
  }

  iterator end()
  {
    return iterator(const_iterator(last, this));
  }

  const_iterator cbegin() const
  {
    return const_iterator(root, this);
  }

  const_iterator cend() const
  {
    return const_iterator(last, this);
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
class LinkedList<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename LinkedList::value_type;
  using difference_type = typename LinkedList::difference_type;
  using pointer = typename LinkedList::const_pointer;
  using reference = typename LinkedList::const_reference;
  Node *current;
private:
  const LinkedList<value_type> *l;
public:
  explicit ConstIterator(Node *current, const LinkedList<value_type> *l):current(current), l(l)
  {}
  reference operator*() const
  {
    if(current->data == nullptr)throw std::out_of_range("-");
    return *(current->data);
  }

  ConstIterator& operator++()
  {
    if(current->next == nullptr)throw std::out_of_range("/");
    current = current->next;
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
    if(current->prev == nullptr)throw std::out_of_range("a");
    current = current->prev;
    return *this;
  }

  ConstIterator operator--(int)
  {
    ConstIterator *pom = this;
    --(*this);
    return *pom;
  }

  ConstIterator operator+(difference_type d)
  {
    for(;d!=0;--d)
    {
      if(current->next == nullptr)throw std::out_of_range("b");
      current = current->next;
    }
    return *this;
  }

  ConstIterator operator-(difference_type d)
  {
    for(;d!=0;--d)
    {
      if(current->prev == nullptr)throw std::out_of_range("v");
      current = current->prev;
    }
    return *this;
  }

  bool operator==(const ConstIterator& other) const
  {
    if(current == other.current)return true;
    return false;
  }

  bool operator!=(const ConstIterator& other) const
  {
    if(current != other.current)return true;
    return false;
  }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
  using reference = typename LinkedList::reference;

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

  Iterator operator+(difference_type d)
  {
    return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d)
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

#endif // AISDI_LINEAR_LINKEDLIST_H


