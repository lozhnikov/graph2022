/**
 * @file iterators.hpp
 * @author Mikhail Lozhnikov/Daniil Morozov
 *
 * Реализация итераторов для различных классов графов.
 */

#ifndef INCLUDE_ITERATORS_HPP_
#define INCLUDE_ITERATORS_HPP_

#include <unordered_set>
#include <unordered_map>

using namespace std;
namespace graph {

/**
 * @brief Класс позволяет проитерироваться по всем вершинам в графе.
 *
 * Этот класс является простой оболочкой над итератором словаря
 * в матрице смежности графа.
 */
class VertexIterator {
 public:
  //! Внутренний тип итератора.
  using InternalIteratorType =
      std::unordered_map<size_t, std::unordered_set<size_t>>::const_iterator;

 private:
  //! Текущая позиция.
  InternalIteratorType pos;

 public:
  /**
   * @brief Функция создаёт итератор из текущей позиции.
   * @param pos Текущая позиция.
   */
  explicit VertexIterator(InternalIteratorType pos) :
    pos(pos) {
  }

  /**
   * @brief Оператор сравнения == для итератора.
   * @param other Другой итератор.
   */
  bool operator==(const VertexIterator& other) const {
    return pos == other.pos;
  }

  /**
   * @brief Оператор сравнения == для итератора.
   * @param other Другой итератор.
   */
  bool operator!=(const VertexIterator& other) const {
    return pos != other.pos;
  }

  /**
   * @brief Префиксная операция инкремента для итератора.
   */
  VertexIterator& operator++() {
    pos++;
    return *this;
  }

  /**
   * @brief Постфиксная операция инкремента для итератора.
   */
  VertexIterator operator++(int) {
    VertexIterator retval(*this);
    pos++;
    return retval;
  }

  /**
   * @brief Функция возврящяет номер вершины, на которую
   *        в данный момент указывает итератор.
   */
  size_t operator*() const {
    return pos->first;
  }
};

/**
 * @brief Класс-адаптер, предназначенный для использования в циклах
 *        range-based for.
 */
class VerticesRange {
 private:
  //! Итератор на первую вершину.
  VertexIterator beginIt;
  //! Итератор на вершину "после последней".
  VertexIterator endIt;

 public:
  //! Внутренний тип итератора.
  using InternalIteratorType =
      std::unordered_map<size_t, std::unordered_set<size_t>>::const_iterator;

  /**
    * @brief Конструктор класса.
    * @param beginIt Итератор на начало в словаре матрицы смежности.
    * @param endIt Итератор на конец в словаре матрицы смежности.
    */
  VerticesRange(InternalIteratorType beginIt, InternalIteratorType endIt) :
    beginIt(beginIt),
    endIt(endIt) {
  }

  /**
   * @brief Возвращает итератор на первую вершину.
   */
  VertexIterator begin() const {
    return beginIt;
  }

  /**
   * @brief Возвращает итератор на вершину "после последней".
   */
  VertexIterator end() const {
    return endIt;
  }
};

}  // namespace graph

#endif  // INCLUDE_ITERATORS_HPP_
