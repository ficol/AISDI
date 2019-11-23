#include <cstddef>
#include <cstdlib>
#include <string>
#include <chrono>
#include <iostream>
#include "Vector.h"
#include "LinkedList.h"

namespace
{

template <typename T>
using LinearCollection = aisdi::LinkedList<T>;

template<typename T>
using VectorCollection = aisdi::Vector<T>;

void buildList(LinearCollection<int> &l, int amount)
{
  for(int i = 0; i < amount; i++)
    l.append(i);
}

void buildVector(VectorCollection<int> &v, int amount)
{
  for(int i = 0; i < amount; i++)
    v.append(i);
}

void appendTest(int amount)
{
  LinearCollection<int> l;
  VectorCollection<int> v;
  buildList(l, amount);
  buildVector(v, amount);
  auto start = std::chrono::system_clock::now();
  for(int i = 0; i < amount; i++)
    v.append(i);
  auto end = std::chrono::system_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Vector: speed of append " << amount << " times: " << elapsed.count() << '\n';
  start = std::chrono::system_clock::now();
  for(int i = 0; i < amount; i++)
    l.append(i);
  end = std::chrono::system_clock::now();
  elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "LinkedList: speed of append " << amount << " times: " << elapsed.count() << '\n';
}

void prependTest(int amount)
{
  LinearCollection<int> l;
  VectorCollection<int> v;
  buildList(l, amount);
  buildVector(v, amount);
  auto start = std::chrono::system_clock::now();
  for(int i = 0; i < amount; i++)
    v.prepend(i);
  auto end = std::chrono::system_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Vector: speed of prepend " << amount << " times: " << elapsed.count() << '\n';
  start = std::chrono::system_clock::now();
  for(int i = 0; i < amount; i++)
    l.prepend(i);
  end = std::chrono::system_clock::now();
  elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "LinkedList: speed of prepend " << amount << " times: " << elapsed.count() << '\n';
}

void insertMiddleTest(int amount)
{
  LinearCollection<int> l;
  VectorCollection<int> v;
  buildList(l, amount);
  buildVector(v, amount);
  auto start = std::chrono::system_clock::now();
  auto it = v.begin()+amount/2;
  for(int i = 0; i < amount; i++)
    v.insert(it, i);
  auto end = std::chrono::system_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Vector: speed of insert " << amount << " times: " << elapsed.count() << '\n';
  start = std::chrono::system_clock::now();
  auto it2 = l.begin()+amount/2;
  for(int i = 0; i < amount; i++)
    l.insert(it2, i);
  end = std::chrono::system_clock::now();
  elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "LinkedList: speed of insert " << amount << " times: " << elapsed.count() << '\n';
}

void getToMiddleTest(int amount)
{
  LinearCollection<int> l;
  VectorCollection<int> v;
  buildList(l, amount);
  buildVector(v, amount);
  auto start = std::chrono::system_clock::now();
  for(int i = 0; i < amount; i++){
    auto it = v.begin()+amount/2;
    (void)it;
  }
  auto end = std::chrono::system_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Vector: speed of getting to the middle of collection " << amount << " times: " << elapsed.count() << '\n';
  start = std::chrono::system_clock::now();
  for(int i = 0; i < amount; i++){
    auto it2 = l.begin()+amount/2;
    (void)it2;
  }
  end = std::chrono::system_clock::now();
  elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "LinkedList: speed of getting to the middle of collection " << amount << " times: " << elapsed.count() << '\n';
}

}

int main()
{
  appendTest(10000000);
  prependTest(10000);
  insertMiddleTest(10000);
  getToMiddleTest(10000);
  return 0;
}
