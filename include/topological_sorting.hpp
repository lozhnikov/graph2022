/**
 * @file topological_sorting.hpp
 * @author Eugene Yakimov
 *
 * Реализация метода топологической сортировки.
 */

#ifndef INCLUDE_TOPOLOGICAL_SORTING_HPP_
#define INCLUDE_TOPOLOGICAL_SORTING_HPP_

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <unordered_set>
#include "graph.hpp"
#include "oriented_graph.hpp"
#include "iterators.hpp"


namespace graph {

/**
 * @brief Обход графа в глубину.
 *
 * @tparam T тип графа.
 *
 * @param used список вершин, в которых уже побывали.
 * @param v текущая вершина в обходе..
 * @param points вектор-ответ.
 */

template<class T>

void DfsTopologicalSorting(std::unordered_set<size_t> *used,
                           std::vector<size_t>* points,
                           const T& oriented_graph, size_t v) {
    used->insert(v);
    for (size_t to : oriented_graph.IncomingEdges(v)) {
      if ((*used).find(to) == (*used).end()) {
      DfsTopologicalSorting(used, points, oriented_graph, to);
      }
    }
    (*points).push_back(v);
}

/**
 * @brief Сама топологическая сортировка.
 *
 * @tparam T тип графа.
 *
 * @param points вектор-ответ.
 */

template<class T>

void TopologicalSorting(const T& oriented_graph, std::vector<size_t>* points) {
    std::unordered_set<size_t> used;

    for (size_t v : oriented_graph.Vertices()) {
        if (used.find(v) == (used).end()) {
          DfsTopologicalSorting(&used, points, oriented_graph, v);
        }
    }
}

}  // namespace graph
#endif  // TOPOLOGICAL_SORTING
