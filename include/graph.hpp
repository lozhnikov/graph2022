/**
 * @file graph.hpp
 * @author Mikhail Lozhnikov
 *
 * Реализация класса простого неориентированного графа.
 */

#ifndef INCLUDE_GRAPH_HPP_
#define INCLUDE_GRAPH_HPP_

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>
#include <iterators.hpp>

namespace graph {

/**
 * @brief Простой неориентированный граф.
 */
class Graph {
 public:
  /**
   * @brief Конструктор класса Graph.
   */
  Graph() :
    edges() {
  }

  /**
   * @brief Добавить вершину в граф.
   *
   * @param id Уникальный номер вершины.
   *
   * Функция добавляет вершину с номером id в граф. Если вершина с таким
   * номером в графе уже есть, то функция ничего не делает.
   */
  void AddVertex(size_t id) {
    edges[id];
  }

  /**
   * @brief Добавить ребро в граф.
   *
   * @param id1 Номер первой вершины.
   * @param id2 Номер второй вершины.
   *
   * Функция добавляет ребро, соединяющее вершины id1 и id2 в граф. Если такое
   * ребро в графе уже есть, то функция ничего не делает.
   */
  void AddEdge(size_t id1, size_t id2) {
    edges[id1].insert(id2);
    edges[id2].insert(id1);
  }

  /**
   * @brief Функция проверяет, есть ли вершина в графе.
   *
   * @param id Номер вершины.
   *
   * Функция возвращает true, если вершина id есть в графе, в противном случае
   * функция возвращает false.
   */
  bool HasVertex(size_t id) const {
    return edges.find(id) != edges.end();
  }

  /**
   * @brief Функция проверяет, есть ли ребро в графе.
   *
   * @param id1 Номер первой вершины.
   * @param id2 Номер второй вершины.
   *
   * Функция возвращает true, если ребро, соединяющее вершины id1 и id2 есть
   * в графе, в противном случае функция возвращает false.
   */
  bool HasEdge(size_t id1, size_t id2) const {
    auto it = edges.find(id1);

    if (it == edges.end())
      return false;

    return it->second.find(id2) != it->second.end();
  }

  /**
   * @brief Удалить вершину из графа.
   *
   * @param id Номер вершины.
   *
   * Функция удаляет вершину с номером id из графа. Если вершины с таким
   * номером в графе нет, то функция ничего не делает.
   */
  void RemoveVertex(size_t id) {
    auto it = edges.find(id);

    if (it == edges.end())
      return;

    for (size_t neighbourId : it->second) {
      if (id != neighbourId) {
        edges[neighbourId].erase(id);
      }
    }

    edges.erase(id);
  }

  /**
   * @brief Удалить ребро из графа.
   *
   * @param id1 Номер первой вершины.
   * @param id2 Номер второй вершины.
   *
   * Функция удаляет ребро, соединяющее вершины id1 и id2 из графа. Если такого
   * ребра в графе нет, то функция ничего не делает.
   */
  void RemoveEdge(size_t id1, size_t id2) {
    if (edges.find(id1) != edges.end())
      edges[id1].erase(id2);

    if (edges.find(id2) != edges.end())
      edges[id2].erase(id1);
  }

  /**
   * @brief Получить множество вершин, с которыми соединена указанная вершина.
   *
   * @param id Номер вершины.
   *
   * Функция возвращает множество вершин в виде хеш-таблицы, с которыми
   * соединена вершина id, то есть множество таких вершин V, что ребро (id, V)
   * присутствует в графе. Если указанной вершини в графе нет, то функция
   * выбрасывает исключение std::out_of_range.
   */
  const std::unordered_set<size_t>& Edges(size_t id) const {
    return edges.at(id);
  }

  /**
   * @brief Получить множество вершин, соединённых ребром с указанной вершиной.
   *
   * @param id Номер вершины.
   *
   * Функция возвращает множество вершин в виде хеш-таблицы, соединённых
   * с вершиной id, то есть множество таких вершин V, что ребро (V, id)
   * присутствует в графе. Если указанной вершини в графе нет, то функция
   * выбрасывает исключение std::out_of_range.
   */
  const std::unordered_set<size_t>& IncomingEdges(size_t id) const {
    return edges.at(id);
  }

  /**
   * @brief Функция для итерирования по вершинам графа.
   *
   * Функция возвращает специальный класс-промежуток, по которому
   * можно проитерироваться при помощи цикла range-base for.
   */
  VerticesRange Vertices() const {
    return VerticesRange(edges.begin(), edges.end());
  }

  /**
   * @brief Функция возвращает количество вершин в графе.
   */
  size_t NumVertices() const {
    return edges.size();
  }

 private:
  /**
   * @brief Вспомогательная функция для получения идентификатора ребра.
   *
   * @param id1 Номер первой вершины.
   * @param id2 Номер второй вершины.
   *
   * Функция возвращает внутренний идентификатор ребра, используемый в классе.
   */
  std::pair<size_t, size_t> MakeEdgeId(size_t id1, size_t id2) const {
    if (id1 < id2)
      return std::make_pair(id1, id2);
    else
      return std::make_pair(id2, id1);
  }

  //! Разреженная матрица связности.
  std::unordered_map<size_t, std::unordered_set<size_t>> edges;
};

}  // namespace graph

#endif  // INCLUDE_GRAPH_HPP_
