#include <cstddef>
#include <cstdlib>
#include <string>
#include <chrono>
#include <iostream>
#include <ctime>
#include <unistd.h>
#include "HashMap.h"
#include "TreeMap.h"

namespace
{

template <typename KeyType, typename ValueType>
using HashMap = aisdi::HashMap<KeyType, ValueType>;

template <typename KeyType, typename ValueType>
using TreeMap = aisdi::TreeMap<KeyType, ValueType>;

void buildHash(HashMap<int, std::string> &h, int size)
{
  for(int i = 0; i < size; i++)
  {
    h[rand()%size] = "aaaa";
  }
}

void buildTree(TreeMap<int, std::string> &t, int size)
{
  for(int i = 0; i < size; i++)
  {
    t[rand()%size] = "aaaa";
  }
}
}
void findTest(int amount, int size)
{
  HashMap<int, std::string> h;
  TreeMap<int, std::string> t;
  for(int i = 0; i < size; i++)
  {
    h[i] = "aaaa";
  }
  for(int i = 0; i < size; i++)
  {
    t[i] = "aaaa";
  }
  auto start = std::chrono::system_clock::now();
  for(int i = 0; i < amount; i++)
    h.find(rand()%size);
  auto end = std::chrono::system_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "HashMap: collection size: " << size << " operation find used: " << amount << " speed: " << elapsed.count() << '\n';
  start = std::chrono::system_clock::now();
  for(int i = 0; i < amount; i++)
    t.find(rand()%size);
  end = std::chrono::system_clock::now();
  elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "TreeMap: collection size: " << size << " operation find used: " << amount << " speed: " << elapsed.count() << '\n';
}

void buildTest(int size)
{
  HashMap<int, std::string> h;
  TreeMap<int, std::string> t;
  auto start = std::chrono::system_clock::now();
  buildHash(h, size);
  auto end = std::chrono::system_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "HashMap: collection size: " << size << " build speed: " << elapsed.count() << '\n';
  start = std::chrono::system_clock::now();
  buildTree(t, size);
  end = std::chrono::system_clock::now();
  elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "TreeMap: collection size: " << size << " build speed: " << elapsed.count() << '\n';
}
int main()
{
  srand(time(nullptr));
  buildTest(10000);
  buildTest(100000); 
  buildTest(1000000);
  buildTest(5000000);
  findTest(1000000, 10000);
  findTest(1000000, 100000);
  findTest(1000000, 1000000);
  findTest(1000000, 5000000);
  return 0;
}

