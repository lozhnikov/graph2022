/**
 * @file ingeborgadapkunaite.hpp
 * @author Nikita Gordeev
 *
 * Реализация алгоритма Куна.
 */
#ifndef INCLUDE_INGEBORGADAPKUNAITE_HPP_
#define INCLUDE_INGEBORGADAPKUNAITE_HPP_

#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <iostream>
#include <vector>
#include <iterators.hpp>
#include "graph.hpp"

namespace graph {
bool TryKuhn(size_t v, std::vector<char>& used,
             std::unordered_map<size_t, std::vector<size_t>>& g,
             std::vector<int>& mt);
bool isBipartite(std::vector<std::vector<size_t>> &graph,
                 std::vector<int>& color);
bool checkDFS(size_t node, std::vector<std::vector<size_t>> adj,
              std::vector<int> &color);

/**
 * @brief Алгоритм Куна нахождения наибольшего паросочетания
 * в двудольном графе
 *
 * @param graph двудольный граф
 */
Graph Ingeborgadapkunaite(const Graph Tolstoi) {
  size_t numv = 0;
  std::unordered_map<size_t, size_t> vertToId;
  std::unordered_map<size_t, size_t> idToVert;
  for (size_t v : Tolstoi.Vertices()) {
    vertToId[v] = numv;
    idToVert[numv] = v;
    numv++;
  }
  std::vector<std::vector<size_t>> graph1(numv);
  for (size_t v : Tolstoi.Vertices())
    for (size_t neighbour : Tolstoi.Edges(v))
      graph1[vertToId[v]].push_back(vertToId[neighbour]);

  std::vector<int> color(numv, -1);
  if (!isBipartite(graph1, color))
    throw "graph is not Bipartite";
  std::unordered_map<size_t, std::vector<size_t>> g1;
  std::unordered_map<size_t, std::vector<size_t>> g2;
  std::unordered_map<size_t, size_t> iDtoG1;
  std::unordered_map<size_t, size_t> iDtoG2;
  std::unordered_map<size_t, size_t> g1toID;
  std::unordered_map<size_t, size_t> g2toID;
  size_t it1 = 0, it2 = 0;
  for (size_t i = 0; i < numv; i++) {
    if (color[i]) {
      g1[it1] = graph1[i];
      iDtoG1[i] = it1;
      g1toID[it1] = i;
      it1++;
    } else {
      g2[it2] = graph1[i];
      iDtoG2[i] = it2;
      g2toID[it2] = i;
      it2++;
    }
  }
  for (size_t i = 0; i < it1; i++)
    for (size_t ii = 0; ii < g1[i].size(); ii++)
      g1[i][ii] = iDtoG2[g1[i][ii]];
  // for (size_t i = 0; i < it2; i++)
    // for (size_t ii = 0; ii < g2[i].size(); ii++)
      // g2[i][ii] = iDtoG1[g2[i][ii]];

    size_t n = g1.size();
    size_t k = numv - n;
    std::vector<int> mt(k, -1);
    std::vector<char> used;
    for (size_t j = 0; j < n; ++j) {
      used.assign(n, false);
      TryKuhn(j, used, g1, mt);
    }
    Graph res;
    for (size_t i = 0; i < k; ++i)
      if (mt[i] != -1)
        res.AddEdge(idToVert[g1toID[size_t(mt[i])]], idToVert[g2toID[i]]);
    return res;
}

/**
 * @brief Рекурсивная функция для алгоритма Куна
 *
 * @param текущая вершина проверки
 * @param  массив пройденных вершин 
 * @param граф
 * @param массив паросочетаний
 */
bool TryKuhn(size_t v, std::vector<char>& used,
             std::unordered_map<size_t, std::vector<size_t>>& g,
             std::vector<int>& mt) {
    if (used[v])
      return false;
    used[v] = true;
    for (size_t i = 0; i < g[v].size(); ++i) {
      size_t to = g[v][i];
      if ((mt[to] == -1) || (TryKuhn(size_t(mt[to]), used, g, mt))) {
        mt[to] = static_cast<int>(v);
        return true;
      }
    }
    return false;
  }

/**
 * @brief Функция проверки на двудольность
 *
 * @param граф
 * @param массив цветов вершин
 */
bool isBipartite(std::vector<std::vector<size_t>> &graph,
                 std::vector<int>& color) {
  size_t n = graph.size();
  for (size_t  i = 0; i < n; i++) {
    if ((color[i] == -1) && (checkDFS(i, graph, color) == false))
      return false;
  }
  return true;
}

/**
 * @brief Рекурсивная функция проверки двудольности
 *
 * @param текущая вершина проверки
 * @param граф
 * @param массив цветов вершин
 */
bool checkDFS(size_t node, std::vector<std::vector<size_t>> adj,
              std::vector<int> &color) {
  if (color[node] == -1)
    color[node] = 1;

  for (size_t v : adj[node]) {
    if (color[v] == -1) {
        color[v] = 1 - color[node];
        if (checkDFS(v, adj, color) == false)
            return false;
    } else if (color[v] == color[node]) {
        return false;
    }
  }
  return true;
}

}  // namespace graph

#endif  // INCLUDE_INGEBORGADAPKUNAITE_HPP_
