#ifndef INCLUDE_MAXIMAL_HPP_
#define INCLUDE_MAXIMAL_HPP_

#include <vector>
#include <iostream>
#include <algorithm>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include "iterators.hpp"
#include "graph.hpp"
#include "weighted_graph.hpp"

namespace graph {

/**
 * @brief Алгоритм Крусакала.
 *
 * @tparam T Тип данных сортируемых элементов.
 *
 * @param data Массив сортируемых элементов.
 * @param m Количество ребер в массиве.
 * @param n Количество вершин в массиве.
 *
 * Функция находит такое поддерево этого графа, 
 * которое бы соединяло все его вершины, и при этом 
 * обладало наименьшим весом
 */

template<typename T>
// тип данных для веса вершин
Graph Maximal(T* data, size_t m, size_t n) {
  std::vector < std::pair < T, std::pair < size_t, size_t > > > g(m);
  // вес - вершина 1 - вершина 2

  for (size_t k = 0; k < m; k++) {
    g[k].first = data[k*3];
    g[k].second.first = static_cast<size_t>(data[k*3+1]);
    g[k].second.second = static_cast<size_t>(data[k*3+2]);
  }
  // заполнение вектора для сортировки
  // структура граф не подойдет т.к. алгоритм работает
  // для сортированных веторов
  sort(g.begin(), g.end());

  T cost = 0;
  Graph res;

  std::vector<size_t> tree_id(n);
  for (size_t i = 0; i < n; ++i)
    tree_id[i] = i;
  // для каждой вершины хранит номер дерева, которому она принадлежит

  for (size_t i = 0; i < m; ++i) {
    size_t a = g[i].second.first,  b = g[i].second.second;
    T l = g[i].first;

    // определяем, принадлежат ли его концы разным деревьям
    if (tree_id[a] != tree_id[b]) {
      cost += l;
      res.AddEdge(a, b);
      size_t old_id = tree_id[b],  new_id = tree_id[a];

      // объединение двух деревьев
      for (size_t j = 0; j < n; ++j)
        if (tree_id[j] == old_id)
          tree_id[j] = new_id;
    }
  }


  return res;
}

}   // namespace graph

#endif  // INCLUDE_MAXIMAL_HPP_
