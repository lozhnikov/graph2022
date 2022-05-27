#ifndef INCLUDE_BELMANF_HPP_
#define INCLUDE_BELMANF_HPP_
#include <utility>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "iterators.hpp"
#include "weighted_oriented_graph.hpp"
  namespace graph {
    /**
 * @brief Алгоритм Беллмана-Форда.
 *
 * @param data Массив сортируемых элементов.
 * @param m Количество ребер в массиве.
 * @param n Количество вершин в массиве.
 * @param v Вершина.
 *
 * Функция находит длины кратчайших путей от вершины v до всех остальных вершин.
 */
  template<typename T>
  std::vector<T> Belmanf(T* data, size_t m, size_t n, size_t v) {
    WeightedOrientedGraph<T> wog;

    for (size_t k = 0; k < m; k++) {
      wog.AddEdge(static_cast<size_t>(data[k*3 + 1]),
                  static_cast<size_t>(data[k*3 + 2]), data[k*3]);
    }

    std::vector<T> res(n);
    const int INF = 1000000000;
    for (size_t i = 0; i < n; i++)
      res[i] = INF;
    res[v] = 0;
    std::vector <std::pair<size_t, size_t>> edges;
    for (size_t v1 : wog.Vertices()) {
      for (size_t neighbour : wog.Edges(v1)) {
        if (res[v1] < INF)
          res[neighbour] = std::min(res[neighbour],
                              res[v1] + wog.EdgeWeight(v1, neighbour));
      }
    }
    /*for (size_t i = 0; i < n-1; ++i)
      for (size_t j = 0; j < m; ++j)
        if (d[e[j].second.first] < INF)
          d[e[j].second.second] = std::min(d[e[j].second.second],
                              d[e[j].second.first] + e[j].first);*/
    return res;
}

}   // namespace graph

#endif  // INCLUDE_BELMANF_HPP_
