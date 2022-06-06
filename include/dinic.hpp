/**
 * @file dinic.hpp
 * @author Denis Maslenikov
 *
 * Реализация алгоритма Диница.
 */

#ifndef INCLUDE_DINIC_HPP_
#define INCLUDE_DINIC_HPP_

#include <cstddef>
#include <cassert>
#include <unordered_map>
#include <list>
#include <stdexcept>
#include <limits>
#include <vector>
#include "weighted_oriented_graph.hpp"

namespace graph {

template<typename Weight>
void Dinic(const graph::WeightedOrientedGraph<Weight> &g,
  graph::WeightedOrientedGraph<Weight> &result,
  const size_t s, const size_t t);
template<typename Weight>
bool BFS(const size_t cur, graph::WeightedOrientedGraph<Weight> &level,
  graph::WeightedOrientedGraph<Weight> &result,
  const size_t s, const size_t t, Weight &min);



template<typename Weight>
/**
 * @brief Алгоритма Диница.
 *
 * @tparam Weigh тип графа.
 *
 * @param s исток.
 * @param t сток.
 */
void Dinic(const graph::WeightedOrientedGraph<Weight> &g,
  graph::WeightedOrientedGraph<Weight> &result,
  const size_t s, const size_t t) {
#ifdef DINIC_DEBUG
  printf("\nDINIC\n");
  printf("SIZE: %d\n", g.NumVertices());
  printf("INPUT:\n");
  for (size_t v : g.Vertices())
  for (size_t e : g.Edges(v))
    printf("Edge: (%zu, %zu, %zu)\n",
      v, e, g.EdgeWeight(v, e));
#endif  // DINIC_DEBUG

  std::vector<std::pair<size_t, size_t>> edges;
  std::vector<size_t> verts;

  for (size_t v : result.Vertices()) {
    for (size_t e : result.Edges(v))
      edges.push_back(std::pair<size_t, size_t>(v, e));
    verts.push_back(v);
  }

  for (auto e : edges)
    result.RemoveEdge(std::get<0>(e), std::get<1>(e));

  for (size_t v : verts)
    result.RemoveVertex(v);

  // Создать копию графа g с нулевыми весами
  for (size_t v : g.Vertices())
    result.AddVertex(v);
  for (size_t v : g.Vertices())
    for (size_t e : g.Edges(v))
      result.AddEdge(v, e, 0);
  assert(g.NumVertices() == result.NumVertices());

  // Построить остаточную сеть с нулевыми весами
  graph::WeightedOrientedGraph<Weight> residual;
  for (size_t v : g.Vertices())
    residual.AddVertex(v);
  for (size_t v : g.Vertices())
    for (size_t e : g.Edges(v)) {
      residual.AddEdge(v, e, 0);
      residual.AddEdge(e, v, 0);
    }

  // Слоистая сеть
  graph::WeightedOrientedGraph<Weight> level;
  for (size_t v : g.Vertices())
    level.AddVertex(v);

  while (true) {
    // Обновить остаточную сеть
    for (size_t v : g.Vertices())
      for (size_t e : g.Edges(v)) {
        residual.EdgeWeight(v, e) = 0;
        residual.EdgeWeight(e, v) = 0;
      }
    for (size_t v : g.Vertices())
      for (size_t e : g.Edges(v)) {
        residual.EdgeWeight(v, e) +=
          g.EdgeWeight(v, e) -
          result.EdgeWeight(v, e);
        residual.EdgeWeight(e, v) +=
         result.EdgeWeight(v, e);
      }
#ifdef DINIC_DEBUG
    printf("\nRESIDUAL:\n");
    for (size_t v : residual.Vertices())
      for (size_t e : residual.Edges(v))
        printf("Edge: (%zu, %zu, %zu)\n", v, e,
            residual.EdgeWeight(v, e));
#endif  // DINIC_DEBUG

    // Обновить слоистую сеть
    std::unordered_map<size_t, int> distance;
    for (size_t v : g.Vertices())
      distance[v] = -1;
    std::list<size_t> queue;
    queue.push_back(s);
    distance[s] = 0;
    while (!queue.empty()) {
    const size_t cur = queue.front();
      queue.pop_front();
      for (size_t child : residual.Edges(cur)) {
        const Weight weight =
          residual.EdgeWeight(cur, child);
        if ((distance[child] == -1 || distance[child] ==
          distance[cur] + 1) &&
          weight > 0) {
          level.AddEdge(cur, child, weight);
          distance[child] = distance[cur] + 1;
          queue.push_back(child);
        }
      }
    }
#ifdef DINIC_DEBUG
    printf("LEVEL:\n");
    for (size_t v : level.Vertices())
      for (size_t e : level.Edges(v))
        printf("Edge: (%zu, %zu, %zu)\n", v, e,
          level.EdgeWeight(v, e));
#endif  // DINIC_DEBUG

    // Прибавить блокирующий поток
    int min = std::numeric_limits<int>::max();
    if (!BFS(s, level, result, s, t, min))
      return;
    do {
      min = std::numeric_limits<int>::max();;
    } while (BFS(s, level, result, s, t, min));

#ifdef DINIC_DEBUG
    printf("RESULT:\n");
    for (size_t v : result.Vertices())
      for (size_t e : result.Edges(v))
        printf("Edge: (%zu, %zu, %zu)\n", v, e,
          result.EdgeWeight(v, e));
#endif// DINIC_DEBUG
	}
}

/**
 * @brief Обход в глубину с выбором минимальног веса.
 */
template<typename Weight>
bool BFS(const size_t cur, graph::WeightedOrientedGraph<Weight> &level,
    graph::WeightedOrientedGraph<Weight> &result,
    const size_t s, const size_t t, Weight &min) {
  if (cur == t)
    return true;
  for (size_t child : level.Edges(cur)) {
    const Weight weight = level.EdgeWeight(cur, child);
    if (weight <= Weight())
      continue;
    if (weight < min)
      min = weight;
    const int tmp = min;
    if (BFS(child, level, result, s, t, min)) {
      level.EdgeWeight(cur, child) -= min;
      try {
        result.EdgeWeight(cur, child) += min;
      } catch (std::out_of_range &e) {
      result.EdgeWeight(child, cur) -= min;
      }
      return true;
    }
    min = tmp;
  }
  return false;
}

}  //  namespace graph

#endif // INCLUDE_DINIC_HPP_
