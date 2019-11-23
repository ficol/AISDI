#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <list>

namespace aisdi
{

template <typename KeyType, typename ValueType>
class HashMap
{
public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

private:
  static const size_type mapSize = 100;
  std::list<value_type>* bucket;

  size_type bucketHash(const key_type &key)const
  {
	  return (int)std::hash<key_type >{}(key) % mapSize;
  }
  
  void deleteMap()
  {
	for(size_type i = 0; i != mapSize; ++i)
	  bucket[i].clear();
  }
  
public:
  HashMap()
  {
    bucket = new std::list<value_type>[mapSize];
  }
  
  ~HashMap()
  {
	if(bucket == nullptr)return;
	deleteMap();
	delete [] bucket;
  }

  HashMap(std::initializer_list<value_type> list)
  {
	bucket = new std::list<value_type>[mapSize];
    for(auto it = list.begin(); it != list.end(); ++it)
      bucket[bucketHash(it->first)].push_back(*it);
  }

  HashMap(const HashMap& other)
  {
	bucket = new std::list<value_type>[mapSize];
    for(auto it = other.begin(); it != other.end(); ++it)
      bucket[bucketHash(it->first)].push_back(*it);
  }

  HashMap(HashMap&& other)
  {
    bucket = other.bucket;
    other.bucket = nullptr;
  }

  HashMap& operator=(const HashMap& other)
  {
    if(this == &other)
      return *this;
    deleteMap();
    for(auto it = other.begin(); it != other.end(); ++it)
      bucket[bucketHash(it->first)].push_back(*it);
    return *this;
  }

  HashMap& operator=(HashMap&& other)
  {
    deleteMap();
    delete [] bucket;
    bucket = other.bucket;
    other.bucket = nullptr;
    return *this;
  }

  bool isEmpty() const
  {
	if(bucket == nullptr)return true;
	for(size_type i = 0; i != mapSize; ++i)
	{
      if(!bucket[i].empty())
        return false;
    }
    return true;
  }
 
  mapped_type& operator[](const key_type& key)
  {
	size_type bucketNumber = bucketHash(key);
    for(auto it = bucket[bucketNumber].begin(); it != bucket[bucketNumber].end(); ++it)
    {
      if(it->first == key)
	    return it->second;
	}
	bucket[bucketNumber].push_back(std::make_pair(key, mapped_type{}));
	return (--bucket[bucketNumber].end())->second;
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    size_type bucketNumber = bucketHash(key);
    for(auto it = bucket[bucketNumber].begin(); it != bucket[bucketNumber].end(); ++it)
    {
      if(it->first == key)
	    return it->second;
	}
	throw std::out_of_range("");
  }

  mapped_type& valueOf(const key_type& key)
  {
    size_type bucketNumber = bucketHash(key);
    for(auto it = bucket[bucketNumber].begin(); it != bucket[bucketNumber].end(); ++it)
    {
      if(it->first == key)
	    return it->second;
	}
	throw std::out_of_range("");
  }

  const_iterator find(const key_type& key) const
  {
    size_type bucketNumber = bucketHash(key);
    for(auto it = bucket[bucketNumber].begin(); it != bucket[bucketNumber].end(); ++it)
    {
      if(it->first == key)
	    return ConstIterator(this, bucketNumber, it);
	}
	return cend();
  }

  iterator find(const key_type& key)
  {
    size_type bucketNumber = bucketHash(key);
    for(auto it = bucket[bucketNumber].begin(); it != bucket[bucketNumber].end(); ++it)
    {
      if(it->first == key)
	    return Iterator(this, bucketNumber, it);
	}
	return end();
  }

  void remove(const key_type& key)
  {
    if(isEmpty())
      throw std::out_of_range("c");
    size_type bucketNumber = bucketHash(key);
    for(auto it = bucket[bucketNumber].begin(); it != bucket[bucketNumber].end(); ++it)
    {
      if(it->first == key)
      {
	    bucket[bucketNumber].erase(it);
	    return;
	  }
	}
	throw std::out_of_range("d");
  }

  void remove(const const_iterator& it)
  {
    if(bucket[it.mapIndex].end() == it.listIterator)
      throw std::out_of_range("d");
    bucket[it.mapIndex].erase(it.listIterator);
  }

  size_type getSize() const
  {
    size_type size = 0;
    for(size_type i = 0; i != mapSize; ++i)
      size += bucket[i].size();
    return size;
  }

  bool operator==(const HashMap& other) const
  {
    for(auto it = begin(), otherIt = other.begin(); it != end() && otherIt != end(); ++it, ++otherIt)
    {
      if(*it != *otherIt)
        return false;
    }
    return true;
  }

  bool operator!=(const HashMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    for(size_type i = 0; i != mapSize; ++i)
    {
      if(!bucket[i].empty())
        return Iterator(this, i, bucket[i].begin());
    }
    return end();
  }

  iterator end()
  {
      return Iterator(this, mapSize-1, bucket[mapSize-1].end());
  }

  const_iterator cbegin() const
  {
    for(size_type i = 0; i != mapSize; ++i)
    {
      if(!bucket[i].empty())
        return ConstIterator(this, i, bucket[i].begin());
    }
    return cend();
  }

  const_iterator cend() const
  {
    return ConstIterator(this, mapSize-1, bucket[mapSize-1].end());
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

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename HashMap::value_type;
  using pointer = const typename HashMap::value_type*;
  using list_iterator = typename std::list<value_type>::iterator;
  const HashMap *map;
  size_type mapIndex;
  list_iterator listIterator;
  explicit ConstIterator(const HashMap *_map, size_type _mapIndex, list_iterator _listIterator) : map(_map), mapIndex(_mapIndex), listIterator(_listIterator)
  {}

  ConstIterator(const ConstIterator& other) : map(other.map), mapIndex(other.mapIndex), listIterator(other.listIterator)
  {}

  ConstIterator& operator++()
  {
    if(*this == map->end())
      throw std::out_of_range("f");
    ++listIterator;
    if(listIterator == map->bucket[mapIndex].end())
    {
		for(size_type i = mapIndex+1; i != map->mapSize; ++i)
		{
		  if(!map->bucket[i].empty())
		  {
		    listIterator = map->bucket[i].begin();
		    mapIndex = i;
		    return *this;
		  }
		}
		mapIndex = mapSize-1;
		listIterator = map->bucket[mapSize-1].end();
	}
	return *this;
  }

  ConstIterator operator++(int)
  {
    ConstIterator *tmp = this;
    ++(*this);
    return *tmp;
  }

  ConstIterator& operator--()
  {
    if(listIterator == map->bucket[mapSize-1].end())
    {
	  	for(int i = mapSize-1; i >= 0; --i)
        {
          if(!map->bucket[i].empty())
          {
	    	  listIterator = --map->bucket[i].end();
	    	  mapIndex = i;
	    	  return *this;
	      }
	    }
	    throw std::out_of_range("h");
	}
    if(listIterator == map->bucket[mapIndex].begin())
    {
      for(int i = mapIndex-1; i >= 0; --i)
      {
        if(!map->bucket[i].empty())
        {
	  	  listIterator = map->bucket[i].end();
	  	  mapIndex = i;
	  	  return *this;
	    }
	  }
	  throw std::out_of_range("h");
	}
	--listIterator;
    return *this;
  }

  ConstIterator operator--(int)
  {
    ConstIterator *tmp = this;
    --(*this);
    return *tmp;
  }

  reference operator*() const
  {
    if(listIterator == map->bucket[mapSize-1].end())
      throw std::out_of_range("z");
    return *listIterator;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    if(map == other.map && mapIndex == other.mapIndex && listIterator == other.listIterator)
      return true;
    return false;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::reference;
  using pointer = typename HashMap::value_type*;
  using list_iterator = typename std::list<value_type>::iterator;
  explicit Iterator(const HashMap *map, size_type mapIndex, list_iterator listIterator) : ConstIterator(map, mapIndex, listIterator)
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

  pointer operator->() const
  {
    return &this->operator*();
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif /* AISDI_MAPS_HASHMAP_H */
