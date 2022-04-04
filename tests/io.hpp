/**
 * @file tests/io.hpp
 * @author Mikhail Lozhnikov
 *
 * Перегруженные операторы для вывода структур данных в поток.
 */

#ifndef TESTS_IO_HPP_
#define TESTS_IO_HPP_

#include <ostream>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <utility>

template<typename ElemType>
std::ostream& operator<<(std::ostream& out, const std::vector<ElemType>& vec) {
  if (vec.empty()) {
    out << "[ ]";
    return out;
  }

  out << "[ " << vec[0];

  for (size_t i = 1; i < vec.size(); i++)
    out << ", " << vec[i];

  out << " ]";

  return out;
}

template<typename ElemType>
std::ostream& operator<<(std::ostream& out, const std::list<ElemType>& l) {
  if (l.empty()) {
    out << "[ ]";
    return out;
  }

  out << "[ " << *l.begin();

  for (auto it = std::next(l.begin()); it != l.end(); ++it)
    out << ", " << *it;

  out << " ]";

  return out;
}


template<typename FirstType, typename SecondType>
std::ostream& operator<<(std::ostream& out,
                         const std::pair<FirstType, SecondType>& p) {
  out << "(" << p.first << ", " << p.second << ")";

  return out;
}

template<typename KeyType, typename ValueType>
std::ostream& operator<<(std::ostream& out,
                         const std::map<KeyType, ValueType>& dictionary) {
  if (dictionary.empty()) {
    out << "{ }";
    return out;
  }

  auto it = dictionary.begin();

  out << "{ " << *it;

  ++it;

  for (; it != dictionary.end(); ++it)
    out << ", " << *it;

  out << " }";

  return out;
}

template<typename KeyType, typename ValueType>
std::ostream& operator<<(std::ostream& out,
    const std::unordered_map<KeyType, ValueType>& dictionary) {
  if (dictionary.empty()) {
    out << "{ }";
    return out;
  }

  auto it = dictionary.begin();

  out << "{ " << *it;

  ++it;

  for (; it != dictionary.end(); ++it)
    out << ", " << *it;

  out << " }";

  return out;
}


#endif  // TESTS_IO_HPP_
