#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace aisdi
{

template <typename KeyType, typename ValueType>
class TreeMap
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
  struct Node
  {
	value_type data;
	Node *left, *right, *parent;
	int height;
	Node():left(nullptr), right(nullptr), parent(nullptr), height(0)
	{};
	Node(value_type _data):data(_data), left(nullptr), right(nullptr), parent(nullptr), height(0)
	{};
  };
  Node *root;
  size_type size;
  
  Node* add(value_type newData)
  {
	if(root->left == nullptr)
	{
	  root->left = new Node(newData);
	  root->left->parent = root;
      return root->left;
	}
	Node *tmp = root->left;
	Node *tmp2 = root;
    while(tmp != nullptr)
	{
      tmp2 = tmp;
  	  if(newData.first >= tmp->data.first)
  	  {
	    tmp = tmp->right;
	  }
	  else
	  {
	    tmp = tmp->left;
	  }
	}
	tmp = new Node(newData);
	tmp->parent = tmp2;
	if(newData.first >= tmp2->data.first)
	  tmp2->right = tmp;
	else tmp2->left = tmp;
	rebalanceAdd(tmp);
	return tmp;
  }
  
  void rebalanceAdd(Node *current)
  {
    for(auto tmp = current->parent;tmp != root; tmp = tmp->parent)
    {
	  if(tmp->left == current)
	  {
		--tmp->height;
	  }
	  if(tmp->right == current)
	  {
	    ++tmp->height;
	  }
	  if(tmp->height == 0)
		break;
      if(tmp->height == 2)
      {
		if(tmp->right->height == -1)
		  tmp = rotateRightLeft(tmp);
		else
		  tmp = rotateLeft(tmp);
		break;
	  }
	  if(tmp->height == -2)
	  {
		if(tmp->left->height == 1)
		  tmp = rotateLeftRight(tmp);
		else
		  tmp = rotateRight(tmp); 
		break;
	  }
	  current = tmp;
	}
  }
  
  void rebalanceRemove(Node *current)
  {
    for(auto tmp = current->parent;tmp != root; tmp = tmp->parent)
    {
	  int a = 2;
	  if(tmp->left == current)
	  {
		++tmp->height;
	  }
	  if(tmp->right == current)
	  {
	    --tmp->height;
	  }
	  if(tmp->height == 1 || tmp->height == -1)
		break;
	  if(tmp->height == 2)
      {
		a = tmp->right->height;
		if(tmp->right->height == -1)
		  tmp = rotateRightLeft(tmp);
		else
		  tmp = rotateLeft(tmp);
	  }
	  else if(current->height == -2)
	  {
		a = tmp->left->height;
		if(tmp->left->height == 1)
		  tmp = rotateLeftRight(tmp); 
		else
		  tmp = rotateRight(tmp);
	  }
	  if(a == 0)
	    break;
	  current = tmp;
	}
  }
  Node* rotateLeft(Node *current)
  {
	Node *tmp = current->right;
	current->right = tmp->left;
	if(tmp->left != nullptr)
	  tmp->left->parent = current;
	if(current == current->parent->left)
	  current->parent->left = tmp;
	else
	  current->parent->right = tmp;
	tmp->left = current;
	tmp->parent = current->parent;
	current->parent = tmp;
	if(tmp->height == 1)
	{
	  tmp->height = 0;
	  current->height = 0;	
	}
	else
	{
	  tmp->height = -1;
	  current->height = 1;	
	}
	return tmp;
  }
  
  Node* rotateRight(Node *current)
  {
	Node *tmp = current->left;
	current->left = tmp->right;
	if(tmp->right != nullptr)
	  tmp->right->parent = current;
	if(current == current->parent->left)
	  current->parent->left = tmp;
	else
	  current->parent->right = tmp;
	tmp->right = current;
	tmp->parent = current->parent;
	current->parent = tmp;
	if(tmp->height == -1)
	{
	  tmp->height = 0;
	  current->height = 0;	
	}
	else
	{
	  tmp->height = 1;
	  current->height = -1;	
	}
	return tmp;
  }
  
  Node* rotateLeftRight(Node *current)
  {
    auto tmp = current->left;
    auto tmp2 = tmp->right;
    tmp->right = tmp2->left;
    if(tmp2->left != nullptr)
	  tmp2->left->parent = tmp;
	tmp->parent->left = tmp2;
	tmp2->left = tmp;
	tmp2->parent = tmp->parent;
	tmp->parent = tmp2;
	//
    current->left = tmp2->right;
	if(tmp2->right != nullptr)
	  tmp2->right->parent = current;
	if(current == current->parent->left)
	  current->parent->left = tmp2;
	else
	  current->parent->right = tmp2;
	tmp2->right = current;
	tmp2->parent = current->parent;
	current->parent = tmp2;
	if(tmp2->height == -1)
	{
	  current->height = 1;
	  tmp->height = 0;	
	}
	else if(tmp2->height == 0)
	{
	  current->height = 0;
	  tmp->height = 0;
	}
	else
	{
	  current->height = 0;
	  tmp->height = -1;
	}
	tmp2->height = 0;
	return tmp2;
  }
  
  Node* rotateRightLeft(Node *current)
  {
    auto tmp = current->right;
    auto tmp2 = tmp->left;
    tmp->left = tmp2->right;
    if(tmp2->right != nullptr)
	  tmp2->right->parent = tmp;
	current->right = tmp2;
	tmp2->right = tmp;
	tmp2->parent = tmp->parent;
	tmp->parent = tmp2;
	//
    current->right = tmp2->left;
	if(tmp2->left != nullptr)
	  tmp2->left->parent = current;
	if(current == current->parent->left)
	  current->parent->left = tmp2;
	else
	  current->parent->right = tmp2;
	tmp2->left = current;
	tmp2->parent = current->parent;
	current->parent = tmp2;
	if(tmp2->height == 1)
	{
	  current->height = -1;
	  tmp->height = 0;	
	}
	else if(tmp2->height == 0)
	{
	  current->height = 0;
	  tmp->height = 0;
	}
	else
	{
	  current->height = 0;
	  tmp->height = 1;
	}
	tmp2->height = 0;
	return tmp2;
  }

  void deleteMap(Node *current)
  {
	  if(current == nullptr)
	    return;
	  if(current->left != nullptr)
	    deleteMap(current->left);
	  if(current->right != nullptr)
	    deleteMap(current->right);
	  if(current->parent != nullptr)
	  {
	    if(current->parent->right == current)
	      current->parent->right = nullptr;
	    else current->parent->left = nullptr;  
	  }
	  delete current;
  }

  Node* findLowest(Node *current)
  {
	  Node* minNode = current;
	  Node *minLeft = findLowest(current->left);
	  Node *minRight = findLowest(current->right);
	  if(minLeft->data.first < minNode->data.first)
	    minNode = minLeft;
	  if(minRight->data.first < minNode->data.first)
	    minNode = minRight;
	  return minNode;
  }
  
public:
  TreeMap()
  {
	root = new Node;
	size = 0;  
  }

  ~TreeMap()
  {
    deleteMap(root);
  }
  TreeMap(std::initializer_list<value_type> list) : TreeMap()
  {
	size = list.size();
    for(auto element : list)
    {
      add(element);
    }
  }

  TreeMap(const TreeMap& other) : TreeMap()
  {
	size = other.size;
    for(auto element = other.begin(); element != other.end(); ++element)
    {
      add(*element);
    }
  }

  TreeMap(TreeMap&& other)
  {
    root = other.root;
    size = other.size;
    other.root = nullptr;
    other.size = 0;
  }

  TreeMap& operator=(const TreeMap& other)
  {
	if(&other == this)
	  return *this;
	size = other.size;
	if(root->left != nullptr)
      deleteMap(root->left);
    for(auto element = other.begin(); element != other.end(); ++element)
    {
      add(*element);
    }
    return *this;
  }

  TreeMap& operator=(TreeMap&& other)
  {
    deleteMap(root->left);
    std::swap(root, other.root);
    size = other.size;
    other.size = 0;
    return *this;
  }

  bool isEmpty() const
  {
    if(size == 0)
      return true;
    return false;
  }

  mapped_type& operator[](const key_type& key)
  {
    if(root->left == nullptr)
    {
	  size = 1;
	  auto tmp = add(std::make_pair(key, mapped_type{}));
      return tmp->data.second;
    }
    auto tmp = root->left;
    while(key != tmp->data.first)
    {
	  	if(key >= tmp->data.first)
	  	{
	  	  if(tmp->right == nullptr)
	  	    break;
	  	  tmp = tmp->right;
	  	}
	  	else
	  	{
		  if(tmp->left == nullptr)
		    break;
		  tmp = tmp->left;
		}
	}
	if(key == tmp->data.first)
	  return tmp->data.second;
	++size;
    auto tmp2 = add(std::make_pair(key, mapped_type{}));
    return tmp2->data.second;
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    auto it = find(key);
    return it->second;
  }

  mapped_type& valueOf(const key_type& key)
  {
    auto it = find(key);
    if(it != end())
      return it->second;
    throw std::out_of_range("");
  }

  const_iterator find(const key_type& key) const
  {
    if(root->left == nullptr)
      return cend();
    auto tmp = root->left;
    while(key != tmp->data.first)
    {
	  	if(key >= tmp->data.first)
	  	  tmp = tmp->right;
	  	else tmp = tmp->left;
	  	if(tmp == nullptr)
	  	  return ConstIterator(this, root);
	}
	return ConstIterator(this, tmp);
  }

  iterator find(const key_type& key)
  {
    if(root->left == nullptr)
      return end();
    auto tmp = root->left;
    while(key != tmp->data.first)
    {
	  	if(key >= tmp->data.first)
	  	  tmp = tmp->right;
	  	else tmp = tmp->left;
	  	if(tmp == nullptr)
	  	  return Iterator(this, root);
	}
	return Iterator(this, tmp);
  }
  
  void remove(const key_type& key)
  {
    if(root->left == nullptr)
     throw std::out_of_range("");
    auto it = find(key);
    if(it == end()) throw std::out_of_range("");
    --size;
    if(it.current->right == nullptr)
    {
	  rebalanceRemove(it.current);
	  if(it.current->parent->left == it.current)
	    it.current->parent->left = it.current->left;
	  else it.current->parent->right = it.current->left;
	  delete it.current;
	  return;
    }
    if(it.current->right != nullptr)
    {
	  rebalanceRemove(it.current);
	  if(it.current->parent->left == it.current)
	    it.current->parent->left = it.current->right;
	  else it.current->parent->right = it.current->right;
	  delete it.current;
	  return;
    }
    auto tmp = findLowest(it.current->right);
    if(tmp->parent->left == tmp)
	  tmp->parent->left = nullptr;
    else tmp->parent->right = nullptr;
    if(it.current->parent->left == it.current)
      it.current->parent->left = tmp;
    else it.current->parent->right = tmp;
    tmp->left = it.current->left;
    tmp->right = it.current->right;
    rebalanceRemove(tmp);
    delete it.current;
  }

  void remove(const const_iterator& it)
  {
    if(root->left == nullptr || it == end())
     throw std::out_of_range("");
    --size;
    rebalanceRemove(it.current);
    if(it.current->right == nullptr)
    {
	  rebalanceRemove(it.current);
	  if(it.current->parent->left == it.current)
	    it.current->parent->left = it.current->left;
	  else it.current->parent->right = it.current->left;
	  delete it.current;
	  return;
    }
    if(it.current->right != nullptr)
    {
	  rebalanceRemove(it.current);
	  if(it.current->parent->left == it.current)
	    it.current->parent->left = it.current->right;
	  else it.current->parent->right = it.current->right;
	  delete it.current;
	  return;
    }
    auto tmp = findLowest(it.current->right);
    if(tmp->parent->left == tmp)
	  tmp->parent->left = nullptr;
    else tmp->parent->right = nullptr;
    if(it.current->parent->left == it.current)
      it.current->parent->left = tmp;
    else it.current->parent->right = tmp;
    tmp->left = it.current->left;
    tmp->right = it.current->right;
    rebalanceRemove(tmp);
    delete it.current;
  }

  size_type getSize() const
  {
    return size;
  }

  bool operator==(const TreeMap& other) const
  {
    if(size != other.size)
      return false;
    for(auto it = begin(); it != end(); ++it)
    {
	  auto otherIt = other.begin();
	  while(otherIt != other.end() && *otherIt != *it)
	    ++otherIt;
	  if(otherIt == other.end())
	    return false;
	}
	return true;
  }

  bool operator!=(const TreeMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
	if(root->left == nullptr)
	  return end();
    Node *mostLeftNode = root->left;
    while(mostLeftNode->left != nullptr)
      mostLeftNode = mostLeftNode->left;
    return Iterator(this, mostLeftNode);
  }

  iterator end()
  {
    return Iterator(this, root);
  }

  const_iterator cbegin() const
  {
    if(root->left == nullptr)
	  return end();
    Node *mostLeftNode = root;
    while(mostLeftNode->left != nullptr)
      mostLeftNode = mostLeftNode->left;
    return ConstIterator(this, mostLeftNode);
  }

  const_iterator cend() const
  {
    return ConstIterator(this, root);
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
class TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename TreeMap::value_type;
  using pointer = const typename TreeMap::value_type*;

  const TreeMap *map;
  Node *current;

  explicit ConstIterator(const TreeMap *_map, Node *_current) : map(_map), current(_current)
  {}

  ConstIterator(const ConstIterator& other) : map(other.map), current(other.current)
  {}

  ConstIterator& operator++()
  {
    if(current->parent == nullptr)
      throw std::out_of_range("");
    if(current->right != nullptr)
	{
	  current = current->right;
	  while(current->left != nullptr)
	    current = current->left;
	  return *this;
	}
	while(true)
	{
	  if(current->parent->left == current)
	  {
		current = current->parent;
		return *this;
	  }
	  current = current->parent;
	}
  }

  ConstIterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  ConstIterator& operator--()
  {
	if(map->size == 0 || current == map->begin().current)
	  throw std::out_of_range("");
    if(current->left != nullptr)
    {
      current = current->left;
      while(current->right != nullptr)
        current = current->right;
      return *this;
    }
    Node *tmp = current->parent;
    while(tmp != nullptr && current == tmp->left) {
      if(tmp->parent == nullptr)
        throw std::out_of_range("");
      current = tmp;
      tmp = tmp->parent;
    }
    current = tmp;
    return *this;
  }

  ConstIterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  reference operator*() const
  {
	if(current->parent == nullptr)
	  throw std::out_of_range("a");
    return current->data;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    if(current == other.current)
      return true;
    return false;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::reference;
  using pointer = typename TreeMap::value_type*;

  explicit Iterator(const TreeMap *_map, Node *_current) : ConstIterator(_map, _current)
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

#endif /* AISDI_MAPS_MAP_H */



