#ifndef INCLUDE_BELMANF_HPP_
#define INCLUDE_BELMANF_HPP_
#include <utility>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <limits>
#include "iterators.hpp"
#include "weighted_oriented_graph.hpp"
  namespace graph {
    /**
 * @brief Алгоритм Беллмана-Форда.
 *
 * @tparam T Тип данных сортируемых элементов.
 *
 * @param wog Взвешенный ориентируемый граф
 *
 * Функция находит длины кратчайших путей от вершины v до всех остальных вершин.
 */

  // тип данных для веса вершин
  std::unordered_map<size_t , std::pair<bool, int>>
  Belmanf(const WeightedOrientedGraph<int>& wog, size_t v) {
  // заполнение вектора для сортировки

    std::unordered_map<size_t , std::pair<bool, int>> res;
    size_t c = 0;
  for (size_t v2 : wog.Vertices()) {
    if (v2 > c)
      c = v2;
  }
    for (size_t i = 0; i < c; i++)
      if (wog.HasVertex(i))
        res[i].first = 0;
    res[v].second = 0;
    res[v].first = 1;
    for (size_t v1 : wog.Vertices()) {
      for (size_t neighbour : wog.Edges(v1)) {
        if (res[v1].first != 0) {
          if (res[neighbour].first != 0) {
            res[neighbour].second = std::min(res[neighbour].second,
            res[v1].second + wog.EdgeWeight(v1, neighbour));
            res[neighbour].first = 1;
          } else {
            res[neighbour].second = res[v1].second +
            wog.EdgeWeight(v1, neighbour);
            res[neighbour].first = 1;
          }
        }
      }
    }

    return res;
}

}   // namespace graph

#endif  // INCLUDE_BELMANF_HPP_
