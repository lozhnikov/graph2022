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
 * @param wGraph Взвешенный граф
 *
 * Функция находит такое поддерево этого графа, 
 * которое бы соединяло все его вершины, и при этом 
 * обладало наименьшим весом
 */

template<typename T>
// тип данных для веса вершин

Graph Maximal(const WeightedGraph<T>& wGraph) {
  std::vector <std::pair<T, std::pair<size_t, size_t>>> g;
  // вес - вершина 1 - вершина 2

  // заполнение вектора для сортировки
  for (size_t v : wGraph.Vertices()) {
    for (size_t neighbour : wGraph.Edges(v)) {
      if (neighbour > v)
        g.push_back({ wGraph.EdgeWeight(v, neighbour), {v, neighbour}});
    }
  }
  // структура граф не подойдет т.к. алгоритм работает
  // для сортированных веторов
  sort(g.begin(), g.end());

  T cost = 0;
  Graph res;

  size_t n = 1;
  for (size_t v : wGraph.Vertices()) {
    if (v > n)
      n = v;
  }
  std::unordered_map<size_t, size_t> treeId;
  for (size_t i = 0; i <= n; ++i)
    if (wGraph.HasVertex(i))
      treeId[i] = i;
  // для каждой вершины хранит номер дерева, которому она принадлежит

  for (size_t i = 0; i < g.size(); ++i) {
    size_t a = g[i].second.first,  b = g[i].second.second;
    T l = g[i].first;

    // определяем, принадлежат ли его концы разным деревьям
    if (treeId[a] != treeId[b]) {
      cost += l;
      res.AddEdge(a, b);
      size_t oldId = treeId[b],  newId = treeId[a];

      // объединение двух деревьев
      for (size_t j = 0; j <= n; ++j)
        if ((treeId[j] == oldId) && (wGraph.HasVertex(j)))
          treeId[j] = newId;
    }
  }
  /* for (size_t v : res.Vertices()) {
    for (size_t neighbour : res.Edges(v)) {
      std::cout << v << ' ' << neighbour << ' ' ;
    }
  } */

  return res;
}
}   // namespace graph

#endif  // INCLUDE_MAXIMAL_HPP_
