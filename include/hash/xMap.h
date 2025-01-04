/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/*
 * File:   xMap.h
 * Author: ltsach
 *
 * Created on October 11, 2024, 7:08 PM
 */

#ifndef XMAP_H
#define XMAP_H
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <memory.h>
using namespace std;

#include "list/DLinkedList.h"
#include "hash/IMap.h"

/*
 * xMap<K, V>:
 *  + K: key type
 *  + V: value type
 *  For example:
 *      xMap<string, int>: map from string to int
 */
template <class K, class V>
class xMap : public IMap<K, V>
{
public:
  class Entry; // forward declaration

protected:
  DLinkedList<Entry *> *table; // array of DLinkedList objects
  int capacity;                // size of table
  int count;                   // number of entries stored hash-map
  float loadFactor;            // define max number of entries can be stored (< (loadFactor * capacity))
  DLinkedList<int> list_clashes;
  int (*hashCode)(K &, int);          // hasCode(K key, int tableSize): tableSize means capacity
  bool (*keyEqual)(K &, K &);         // keyEqual(K& lhs, K& rhs): test if lhs == rhs
  bool (*valueEqual)(V &, V &);       // valueEqual(V& lhs, V& rhs): test if lhs == rhs
  void (*deleteKeys)(xMap<K, V> *);   // deleteKeys(xMap<K,V>* pMap): delete all keys stored in pMap
  void (*deleteValues)(xMap<K, V> *); // deleteValues(xMap<K,V>* pMap): delete all values stored in pMap

public:
  xMap(
      int (*hashCode)(K &, int), // require
      float loadFactor = 0.75f,
      bool (*valueEqual)(V &, V &) = 0,
      void (*deleteValues)(xMap<K, V> *) = 0,
      bool (*keyEqual)(K &, K &) = 0,
      void (*deleteKeys)(xMap<K, V> *) = 0);

  xMap(const xMap<K, V> &map);                  // copy constructor
  xMap<K, V> &operator=(const xMap<K, V> &map); // assignment operator
  ~xMap();

  // Inherit from IMap:BEGIN
  V put(K key, V value);
  V &get(K key);
  V remove(K key, void (*deleteKeyInMap)(K) = 0);
  bool remove(K key, V value, void (*deleteKeyInMap)(K) = 0, void (*deleteValueInMap)(V) = 0);
  bool containsKey(K key);
  bool containsValue(V value);
  bool empty();
  int size();
  void clear();
  string toString(string (*key2str)(K &) = 0, string (*value2str)(V &) = 0);
  DLinkedList<K> keys();
  DLinkedList<V> values();
  DLinkedList<int> clashes();
  // Inherit from IMap:END

  // Show map on screen: need to convert key to string (key2str) and value2str
  void println(string (*key2str)(K &) = 0, string (*value2str)(V &) = 0)
  {
    cout << this->toString(key2str, value2str) << endl;
  }
  int getCapacity()
  {
    return capacity;
  }

  ///////////////////////////////////////////////////
  // STATIC METHODS: BEGIN
  //      * Used to create xMap objects
  ///////////////////////////////////////////////////
  /*
   * sample hash function for keys of types integer and string:
   */
  static int intKeyHash(int &key, int capacity)
  {
    return key % capacity;
  }
  static int stringKeyHash(string &key, int capacity)
  {
    long long int sum = 0;
    for (int idx = 0; idx < key.length(); idx++)
      sum += key[idx];
    return sum % capacity;
  }
  /*
   * freeKey(xMap<K,V> *pMap):
   *  Purpose: a typical function for deleting keys stored in map
   *  WHEN to use:
   *      1. K is a pointer type; AND
   *      2. Users need xMap to free keys
   */
  static void freeKey(xMap<K, V> *pMap)
  {
    for (int idx = 0; idx < pMap->capacity; idx++)
    {
      DLinkedList<Entry *> list = pMap->table[idx];
      for (auto pEntry : list)
      {
        delete pEntry->key;
      }
    }
  }
  /*
   * freeValue(xMap<K,V> *pMap):
   *  Purpose: a typical function for deleting values stored in map
   *  WHEN to use:
   *      1. V is a pointer type; AND
   *      2. Users need xMap to free values
   */
  static void freeValue(xMap<K, V> *pMap)
  {
    for (int idx = 0; idx < pMap->capacity; idx++)
    {
      DLinkedList<Entry *> list = pMap->table[idx];
      for (auto pEntry : list)
      {
        delete pEntry->value;
      }
    }
  }
  /*
   * deleteEntry(Entry* ptr): a function pointer to delete pointer to Entry
   */
  static void deleteEntry(Entry *ptr)
  {
    delete ptr;
  }
  ///////////////////////////////////////////////////
  // STATIC METHODS: END
  //      * Used to create xMap objects
  ///////////////////////////////////////////////////

protected:
  ////////////////////////////////////////////////////////
  ////////////////////////  UTILITIES ////////////////////
  ////////////////////////////////////////////////////////
  void ensureLoadFactor(int minCapacity);
  // future version:
  //   should add a method to trim table shorter when removing key (and value)
  void rehash(int newCapacity);
  void removeInternalData();
  void copyMapFrom(const xMap<K, V> &map);
  void moveEntries(
      DLinkedList<Entry *> *oldTable, int oldCapacity,
      DLinkedList<Entry *> *newTable, int newCapacity);

  /*
   * keyEQ(K& lhs, K& rhs): verify the equality of two keys
   */
  bool keyEQ(K &lhs, K &rhs)
  {
    if (keyEqual != 0)
      return keyEqual(lhs, rhs);
    else
      return lhs == rhs;
  }
  /*
   *  valueEQ(V& lhs, V& rhs): verify the equality of two values
   */
  bool valueEQ(V &lhs, V &rhs)
  {
    if (valueEqual != 0)
      return valueEqual(lhs, rhs);
    else
      return lhs == rhs;
  }

  //////////////////////////////////////////////////////////////////////
  ////////////////////////  INNER CLASSES DEFNITION ////////////////////
  //////////////////////////////////////////////////////////////////////
public:
  // Entry: BEGIN
  class Entry
  {
  private:
    K key;
    V value;
    friend class xMap<K, V>;

  public:
    Entry(K key, V value)
    {
      this->key = key;
      this->value = value;
    }
  };
  // Entry: END
};

//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template <class K, class V>
xMap<K, V>::xMap(
    int (*hashCode)(K &, int),
    float loadFactor,
    bool (*valueEqual)(V &lhs, V &rhs),
    void (*deleteValues)(xMap<K, V> *),
    bool (*keyEqual)(K &lhs, K &rhs),
    void (*deleteKeys)(xMap<K, V> *pMap))
{
  // Khởi tạo các tham số truyền vào
  this->hashCode = hashCode;
  this->loadFactor = loadFactor;
  this->valueEqual = valueEqual;
  this->deleteValues = deleteValues;
  this->keyEqual = keyEqual;
  this->deleteKeys = deleteKeys;

  // Cấp phát và khởi tạo bảng băm
  this->capacity = 10;
  this->count = 0;
  this->table = new DLinkedList<Entry *>[this->capacity];
}

template <class K, class V>
xMap<K, V>::xMap(const xMap<K, V> &map)
{
  // Sao chép các thuộc tính cơ bản
  this->capacity = map.capacity;
  this->count = map.count;
  this->loadFactor = map.loadFactor;

  // Sao chép các hàm callback
  this->hashCode = map.hashCode;
  this->keyEqual = map.keyEqual;
  this->valueEqual = map.valueEqual;
  // this->deleteKeys = map.deleteKeys;
  //  this->deleteValues = map.deleteValues;
  this->deleteKeys = nullptr;
  this->deleteValues = nullptr;

  // Tạo một bản sao của bảng băm
  this->table = new DLinkedList<Entry *>[this->capacity];

  // Sao chép từng phần tử từ bảng băm của `map`
  for (int i = 0; i < this->capacity; i++)
  {
    DLinkedList<Entry *> &srcList = map.table[i];
    DLinkedList<Entry *> &destList = this->table[i];

    for (auto srcEntry : srcList)
    {
      // Tạo một bản sao của từng `Entry` trong danh sách liên kết
      Entry *newEntry = new Entry(srcEntry->key, srcEntry->value);
      destList.add(newEntry);
    }
  }
}

template <class K, class V>
xMap<K, V> &xMap<K, V>::operator=(const xMap<K, V> &map)
{
  // Kiểm tra gán cho chính nó (self-assignment)
  if (this == &map)
  {
    return *this;
  }

  // Giải phóng dữ liệu hiện tại (nếu có)
  // this->removeInternalData();

  // Sao chép các thuộc tính cơ bản
  this->capacity = map.capacity;
  this->count = map.count;
  this->loadFactor = map.loadFactor;

  // Sao chép các hàm callback
  this->hashCode = map.hashCode;
  this->keyEqual = map.keyEqual;
  this->valueEqual = map.valueEqual;
  this->deleteKeys = nullptr;
  this->deleteValues = nullptr;

  // Tạo bảng băm mới
  this->table = new DLinkedList<Entry *>[this->capacity];

  // Sao chép các phần tử từ `map`
  for (int i = 0; i < this->capacity; i++)
  {
    DLinkedList<Entry *> &srcList = map.table[i];
    DLinkedList<Entry *> &destList = this->table[i];

    for (auto srcEntry : srcList)
    {
      // Tạo bản sao của từng `Entry` và thêm vào danh sách liên kết
      Entry *newEntry = new Entry(srcEntry->key, srcEntry->value);
      destList.add(newEntry);
    }
  }

  return *this; // Trả về đối tượng hiện tại
}

template <class K, class V>
xMap<K, V>::~xMap()
{
  // Giải phóng tài nguyên nội bộ
  this->removeInternalData();
}

//////////////////////////////////////////////////////////////////////
//////////////////////// IMPLEMENTATION of IMap    ///////////////////
//////////////////////////////////////////////////////////////////////

template <class K, class V>
V xMap<K, V>::put(K key, V value)
{
  // Tính toán chỉ số bucket từ hashCode
  int index = this->hashCode(key, this->capacity);

  // Lấy danh sách bucket từ bảng băm
  DLinkedList<Entry *> &bucket = this->table[index];

  // Duyệt qua danh sách để tìm key
  for (auto entry : bucket)
  {
    if (this->keyEQ(entry->key, key)) 
    {
      // Key đã tồn tại, cập nhật value
      V oldValue = entry->value; // Lưu giá trị cũ để trả về
      entry->value = value;      // Cập nhật giá trị mới
      return oldValue;
    }
  }

  // Nếu key chưa tồn tại, thêm một Entry mới vào bucket
  Entry *newEntry = new Entry(key, value);
  bucket.add(newEntry); // Thêm entry mới vào danh sách

  // Tăng số lượng phần tử
  this->count++;

  // Đảm bảo hệ số tải không vượt quá ngưỡng
  if (bucket.size() > 1 && !list_clashes.contains(index))
    list_clashes.add(index);

  if (count > capacity * loadFactor)
  {
    ensureLoadFactor(count + 1);
  }

  // Trả về giá trị đã chèn (giá trị mới)
  return value;
}

template <class K, class V>
V &xMap<K, V>::get(K key)
{
  // Tính toán chỉ số bucket từ hashCode
  int index = hashCode(key, capacity);

  // Lấy danh sách bucket tại chỉ số đó
  DLinkedList<Entry *> &bucket = table[index];

  // Duyệt qua danh sách để tìm key
  for (auto entry : bucket)
  {
    if (keyEQ(entry->key, key))
    {
      // Nếu tìm thấy key, trả về value tương ứng
      return entry->value;
    }
  }

  // Nếu không tìm thấy key, ném ngoại lệ KeyNotFound
  stringstream os;
  os << "key (" << key << ") is not found";
  throw KeyNotFound(os.str());
}

template <class K, class V>
V xMap<K, V>::remove(K key, void (*deleteKeyInMap)(K))
{
  // Tính toán chỉ số bucket từ hashCode
  int index = hashCode(key, capacity);

  // Lấy danh sách bucket tại chỉ số đó
  DLinkedList<Entry *> &bucket = table[index];

  // Duyệt qua danh sách để tìm key
  for (auto it = bucket.begin(); it != bucket.end(); ++it)
  {
    if (keyEQ((*it)->key, key))
    {
      // Nếu tìm thấy key, sao lưu giá trị
      V retValue = (*it)->value;

      // Giải phóng bộ nhớ key nếu deleteKeyInMap khác nullptr
      if (deleteKeyInMap != nullptr)
      {
        deleteKeyInMap((*it)->key);
      }

      // Gỡ Entry khỏi danh sách và giải phóng bộ nhớ của Entry
      bucket.removeItem(*it);
      delete *it; // Giải phóng bộ nhớ Entry

      // Trả về giá trị đã sao lưu
      return retValue;
    }
  }

  // Nếu không tìm thấy key, ném ngoại lệ KeyNotFound
  stringstream os;
  os << "key (" << key << ") is not found";
  throw KeyNotFound(os.str());
}

template <class K, class V>
bool xMap<K, V>::remove(K key, V value, void (*deleteKeyInMap)(K), void (*deleteValueInMap)(V))
{
  // Tính toán chỉ số bucket từ hashCode
  int index = hashCode(key, capacity);

  // Lấy danh sách bucket tại chỉ số đó
  DLinkedList<Entry *> &bucket = table[index];

  // Duyệt qua danh sách để tìm cặp <key, value>
  for (auto it = bucket.begin(); it != bucket.end(); ++it)
  {
    if (keyEQ((*it)->key, key) && valueEQ((*it)->value, value))
    {
      // Nếu tìm thấy cặp <key, value>, giải phóng bộ nhớ nếu cần
      if (deleteKeyInMap != nullptr)
      {
        deleteKeyInMap((*it)->key);
      }
      if (deleteValueInMap != nullptr)
      {
        deleteValueInMap((*it)->value);
      }

      // Gỡ Entry khỏi danh sách và giải phóng bộ nhớ của Entry
      bucket.removeItem(*it);
      delete *it;

      // Trả về true vì đã xóa thành công
      return true;
    }
  }

  // Nếu không tìm thấy cặp <key, value>, trả về false
  return false;
}

template <class K, class V>
bool xMap<K, V>::containsKey(K key)
{
  // Tính toán chỉ số bucket từ hàm băm
  int index = hashCode(key, capacity);

  // Lấy danh sách bucket tại chỉ số đó
  DLinkedList<Entry *> &bucket = table[index];

  // Duyệt qua danh sách để kiểm tra sự tồn tại của key
  for (auto it = bucket.begin(); it != bucket.end(); ++it)
  {
    if (keyEQ((*it)->key, key))
    {
      return true; // Nếu tìm thấy key
    }
  }

  // Không tìm thấy key
  return false;
}

template <class K, class V>
bool xMap<K, V>::containsValue(V value)
{
  // Duyệt qua tất cả các bucket trong bảng băm
  for (int i = 0; i < capacity; ++i)
  {
    // Lấy danh sách liên kết (bucket) tại vị trí i
    DLinkedList<Entry *> &bucket = table[i];

    // Duyệt qua từng Entry trong bucket
    for (auto it = bucket.begin(); it != bucket.end(); ++it)
    {
      if (valueEQ((*it)->value, value))
      {
        return true; // Nếu tìm thấy value
      }
    }
  }

  // Không tìm thấy value
  return false;
}

template <class K, class V>
bool xMap<K, V>::empty()
{
  // YOUR CODE IS HERE
  return count == 0;
}

template <class K, class V>
int xMap<K, V>::size()
{
  // YOUR CODE IS HERE
  return count;
}

template <class K, class V>
void xMap<K, V>::clear()
{
  // Giải phóng bộ nhớ của các Entry
  removeInternalData();

  // Đặt lại trạng thái ban đầu
  capacity = 10;
  count = 0;

  // Khởi tạo lại bảng băm với capacity mới
  table = new DLinkedList<Entry *>[capacity];
}

template <class K, class V>
DLinkedList<K> xMap<K, V>::keys()
{
  // Khởi tạo danh sách để chứa các khóa
  DLinkedList<K> keyList;

  // Duyệt qua tất cả các vị trí trong bảng băm
  for (int i = 0; i < capacity; i++)
  {
    // Lấy danh sách tại vị trí i
    DLinkedList<Entry *> *bucket = table + i;

    // Duyệt qua các phần tử trong danh sách
    for (Entry *entry : *bucket)
    {
      // Thêm khóa vào danh sách kết quả
      keyList.add(entry->key);
    }
  }

  return keyList;
}

template <class K, class V>
DLinkedList<V> xMap<K, V>::values()
{
  // Khởi tạo danh sách để chứa các giá trị
  DLinkedList<V> valueList;

  // Duyệt qua tất cả các vị trí trong bảng băm
  for (int i = 0; i < capacity; i++)
  {
    // Lấy danh sách tại vị trí i
    DLinkedList<Entry *> *bucket = table + i;

    // Duyệt qua các phần tử trong danh sách
    for (Entry *entry : *bucket)
    {
      // Thêm giá trị vào danh sách kết quả
      valueList.add(entry->value);
    }
  }

  return valueList;
}

template <class K, class V>
DLinkedList<int> xMap<K, V>::clashes()
{
  // Khởi tạo danh sách kết quả
  DLinkedList<int> clashList;

  // Duyệt qua tất cả các vị trí trong bảng băm
  for (int i = 0; i < capacity; i++)
  {
    // Lấy danh sách tại chỉ mục i
    DLinkedList<Entry *> *bucket = table + i;

    // Thêm số lượng phần tử trong danh sách vào clashList
    clashList.add(bucket->size());
  }

  return clashList;
}

template <class K, class V>
string xMap<K, V>::toString(string (*key2str)(K &), string (*value2str)(V &))
{
  stringstream os;
  string mark(50, '=');
  os << mark << endl;
  os << setw(12) << left << "capacity: " << capacity << endl;
  os << setw(12) << left << "size: " << count << endl;
  for (int idx = 0; idx < capacity; idx++)
  {
    DLinkedList<Entry *> list = table[idx];

    os << setw(4) << left << idx << ": ";
    stringstream itemos;
    for (auto pEntry : list)
    {
      itemos << " (";

      if (key2str != 0)
        itemos << key2str(pEntry->key);
      else
        itemos << pEntry->key;
      itemos << ",";
      if (value2str != 0)
        itemos << value2str(pEntry->value);
      else
        itemos << pEntry->value;

      itemos << ");";
    }
    string valuestr = itemos.str();
    if (valuestr.length() > 0)
      valuestr = valuestr.substr(0, valuestr.length() - 1);
    os << valuestr << endl;
  }
  os << mark << endl;

  return os.str();
}

////////////////////////////////////////////////////////
//                  UTILITIES
//              Code are provided
////////////////////////////////////////////////////////

/*
 * moveEntries:
 *  Purpose: move all entries in the old hash table (oldTable) to the new table (newTable)
 */
template <class K, class V>
void xMap<K, V>::moveEntries(
    DLinkedList<Entry *> *oldTable, int oldCapacity,
    DLinkedList<Entry *> *newTable, int newCapacity)
{
  for (int old_index = 0; old_index < oldCapacity; old_index++)
  {
    DLinkedList<Entry *> &oldList = oldTable[old_index];
    for (auto oldEntry : oldList)
    {
      int new_index = this->hashCode(oldEntry->key, newCapacity);
      DLinkedList<Entry *> &newList = newTable[new_index];
      newList.add(oldEntry);
    }
  }
}

/*
 * ensureLoadFactor:
 *  Purpose: ensure the load-factor,
 *      i.e., the maximum number of entries does not exceed "loadFactor*capacity"
 */
template <class K, class V>
void xMap<K, V>::ensureLoadFactor(int current_size)
{
  int maxSize = (int)(loadFactor * capacity);

  // cout << "ensureLoadFactor: count = " << count << "; maxSize = " << maxSize << endl;
  if (current_size > maxSize)
  {
    int oldCapacity = capacity;
    // int newCapacity = oldCapacity + (oldCapacity >> 1);
    int newCapacity = 1.5 * oldCapacity;
    rehash(newCapacity);
  }
}

/*
 * rehash(int newCapacity)
 *  Purpose:
 *      1. create a new hash-table with newCapacity, and
 *      2. move all the old table to to new one
 *      3. free the old table.
 */
template <class K, class V>
void xMap<K, V>::rehash(int newCapacity)
{
  DLinkedList<Entry *> *pOldMap = this->table;
  int oldCapacity = capacity;

  // Create new table:
  this->table = new DLinkedList<Entry *>[newCapacity];
  this->capacity = newCapacity; // keep "count" not changed

  moveEntries(pOldMap, oldCapacity, this->table, newCapacity);

  // remove old data: only remove nodes in list, no entry
  for (int idx = 0; idx < oldCapacity; idx++)
  {
    DLinkedList<Entry *> &list = pOldMap[idx];
    list.clear();
  }
  // Remove oldTable
  delete[] pOldMap;
}

/*
 * removeInternalData:
 *  Purpose:
 *      1. Remove all keys and values if users require,
 *          i.e., deleteKeys and deleteValues are not nullptr
 *      2. Remove all entry
 *      3. Remove table
 */
template <class K, class V>
void xMap<K, V>::removeInternalData()
{
  // Remove user's data
  if (deleteKeys != 0)
    deleteKeys(this);
  if (deleteValues != 0)
    deleteValues(this);

  // Remove all entries in the current map
  for (int idx = 0; idx < this->capacity; idx++)
  {
    DLinkedList<Entry *> &list = this->table[idx];
    for (auto pEntry : list)
      delete pEntry;
    list.clear();
  }

  // Remove table
  delete[] table;
}

/*
 * copyMapFrom(const xMap<K,V>& map):
 *  Purpose:
 *      1. Remove all the entries of the current hash-table
 *      2. Copy (Shallow-copy only) all the entries in the input map
 *          to the current table
 */

template <class K, class V>
void xMap<K, V>::copyMapFrom(const xMap<K, V> &map)
{
  // removeInternalData();

  this->capacity = map.capacity;
  this->count = 0;
  this->table = new DLinkedList<Entry *>[capacity];

  this->hashCode = hashCode;
  this->loadFactor = loadFactor;

  this->valueEqual = valueEqual;
  this->keyEqual = keyEqual;
  // SHOULD NOT COPY: deleteKeys, deleteValues => delete ONLY TIME in map if needed

  // copy entries
  for (int idx = 0; idx < map.capacity; idx++)
  {
    DLinkedList<Entry *> &list = map.table[idx];
    for (auto pEntry : list)
    {
      this->put(pEntry->key, pEntry->value);
    }
  }
}
#endif /* XMAP_H */
