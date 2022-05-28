/**
 * @file find_bridges.hpp
 * @author Daniil Morozov
 *
 * Реализация алгоритма поиска точек сочленения.
 */
#ifndef INCLUDE_CUT_POINTS_HPP_
#define INCLUDE_CUT_POINTS_HPP_

#include <optional>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>
#include "graph.hpp"
#include "oriented_graph.hpp"
#include "weighted_graph.hpp"
#include "weighted_oriented_graph.hpp"
#include "iterators.hpp"

namespace graph {

template<class T>
/**
 * @brief Обход графа в глубину.
 *
 * @tparam T тип графа.
 *
 * @param timer описывает времена входа в вершину.
 * @param used список вершин, в которых уже побывали.
 * @param entryTime список времен входа в вершину.
 * @param minTime список времен минимального входа в вершину.
 * @param v текущая вершина в обходе.
 * @param p предок вершины v.
 * @param points вектор-ответ.
 */
void DfsCutPoints(int *timer, std::unordered_set<size_t> *used,
                    std::map<size_t, int> *entryTime,
                    std::map<size_t, int> *minTime,
                    std::vector<size_t> *points, const T& graph, size_t v,
                    std::optional<size_t> opt) {
  used->insert(v);
  (*entryTime)[v] = (*timer)++;
  (*minTime)[v] = (*timer)++;

  int children = 0;
  for (size_t to : graph.IncomingEdges(v)) {
    if (opt) {
      if (to == opt)
        continue;
    }
    if ((*used).find(to) != (*used).end()) {
      (*minTime)[v] = std::min ((*minTime)[v], (*entryTime)[to]);
    } else {
      DfsCutPoints(timer, used, entryTime,
                     minTime, points, graph, to, v);
      (*minTime)[v] = std::min ((*minTime)[v], (*minTime)[to]);
      if ((*minTime)[to] >= (*entryTime)[v] && (opt))
        (*points).push_back(v);
      ++children;
    }
  }
  if ((!opt) && children > 1)
    (*points).push_back(v);
}
/**
 * @brief Сама функция поиска точек сочленения.
 *
 * @tparam T тип графа.
 *
 * @param points вектор-ответ.
 */
template<class T>
void CutPoints(const T& graph, std::vector<size_t>* points) {
  int timer = 0;
  std::unordered_set<size_t> used;
  std::map<size_t, int> entryTime;
  std::map<size_t, int> minTime;

  for (size_t v : graph.Vertices()) {
    if (used.find(v) == (used).end()) {
      std::optional<size_t> opt = std::nullopt;
      DfsCutPoints(&timer, &used, &entryTime,
                     &minTime, points, graph, v, opt);
    }
  }
}
}  // namespace graph
#endif  // INCLUDE_CUT_POINTS_HPP_
