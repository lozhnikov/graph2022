/**
 * @file weighted_oriented_graph.hpp
 * @author Mikhail Lozhnikov
 *
 * Реализация класса взвешенного ориентированного графа.
 */

#ifndef INCLUDE_WEIGHTED_ORIENTED_GRAPH_HPP_
#define INCLUDE_WEIGHTED_ORIENTED_GRAPH_HPP_

#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <iterators.hpp>

namespace graph {

/**
 * @brief Взвешенный ориентированный граф.
 *
 * @tparam Weight Тип веса графа.
 */
template<typename Weight>
class WeightedOrientedGraph {
 public:
  //! Тип данных для веса ребра.
  using WeightType = Weight;

  /**
   * @brief Конструктор класса WeightedOrientedGraph.
   */
  WeightedOrientedGraph() :
    edges(),
    incomingEdges(),
    weights() {
  }

  /**
   * @brief Деструктор класса WeightedOrientedGraph.
   */
  ~WeightedOrientedGraph() {
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
    incomingEdges[id];
  }

  /**
   * @brief Добавить ребро в граф.
   *
   * @param id1 Номер вершины, из которой выходит ребро.
   * @param id2 Номер вершины, в которую входит ребро.
   * @param weight Вес ребра.
   *
   * Функция добавляет ребро с весом weight, выходящее из вершины id1 и входящее
   * в вершину id2 в граф. Если такое ребро в графе уже есть, то функция ничего
   * не делает.
   */
  void AddEdge(size_t id1, size_t id2, Weight weight) {
    edges[id1].insert(id2);
    incomingEdges[id2].insert(id1);
    weights[std::make_pair(id1, id2)] = weight;
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
   * @param id1 Номер вершины, из которой выходит ребро.
   * @param id2 Номер вершины, в которую входит ребро.
   *
   * Функция возвращает true, если ребро, выходящее из вершины id1 и входящее
   * в вершину id2 есть в графе, в противном случае функция возвращает false.
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

    if (it != edges.end()) {
      for (size_t neighbourId : it->second) {
        if (id != neighbourId) {
          incomingEdges[neighbourId].erase(id);
        }

        weights.erase(std::make_pair(id, neighbourId));
      }

      edges.erase(id);
    }

    auto it2 = incomingEdges.find(id);

    if (it2 != incomingEdges.end()) {
      for (size_t neighbourId : it2->second) {
        if (id != neighbourId) {
          edges[neighbourId].erase(id);
        }

        weights.erase(std::make_pair(neighbourId, id));
      }

      incomingEdges.erase(id);
    }
  }

  /**
   * @brief Удалить ребро из графа.
   *
   * @param id1 Номер вершины, из которой выходит ребро.
   * @param id2 Номер вершины, в которую входит ребро.
   *
   * Функция удаляет ребро, выходящее из вершины id1 и входящее в вершину id2
   * из графа. Если такого ребра в графе нет, то функция ничего не делает.
   */
  void RemoveEdge(size_t id1, size_t id2) {
    weights.erase(std::make_pair(id1, id2));

    if (edges.find(id1) != edges.end())
      edges[id1].erase(id2);

    if (incomingEdges.find(id2) != incomingEdges.end())
      incomingEdges[id2].erase(id1);
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
    return incomingEdges.at(id);
  }

  /**
   * @brief Получить вес ребра.
   *
   * @param id1 Номер вершины, из которой выходит ребро.
   * @param id2 Номер вершины, в которую входит ребро.
   *
   * Функция возвращает вес ребра, выходящее из вершины id1 и входящее
   * в вершину id2. Если такого ребра в графе нет, то функция генерирует
   * исключение std::out_of_range.
   */
  const Weight& EdgeWeight(size_t id1, size_t id2) const {
    return weights.at(std::make_pair(id1, id2));
  }

  /**
   * @brief Изменить вес ребра.
   *
   * @param id1 Номер первой вершины.
   * @param id2 Номер второй вершины.
   *
   * Функция возвращает ссылку на вес ребра, соединяющего вершины id1 и id2.
   * Если такого ребра в графе нет, то функция генерирует исключение
   * std::out_of_range.
   */
  Weight& EdgeWeight(size_t id1, size_t id2) {
    return weights.at(std::make_pair(id1, id2));
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
   * @brief Класс для вычисления хеш кода ребра.
   */
  class Hash {
    /**
     * @brief Функция, вычисляющая хеш код ребра.
     *
     * @param id Уникальный идентификатор ребра.
     */
   public:
    size_t operator()(std::pair<size_t, size_t> id) const noexcept {
      const size_t prime = 479001599;

      return id.first * prime + id.second;
    }
  };

  //! Разреженная матрица связности. Словарь исходящих рёбер.
  std::unordered_map<size_t, std::unordered_set<size_t>> edges;

  //! Разреженная матрица связности. Словарь входящих рёбер.
  std::unordered_map<size_t, std::unordered_set<size_t>> incomingEdges;

  //! Словарь весов рёбер.
  std::unordered_map<std::pair<size_t, size_t>, Weight, Hash> weights;
};

}  // namespace graph

#endif  // INCLUDE_WEIGHTED_ORIENTED_GRAPH_HPP_
