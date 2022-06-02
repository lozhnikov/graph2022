/**
 * @file methods/topological_sorting.cpp
 * @author Eugene Yakimov
 *
 * Реализация серверной части метода.
 */

#ifndef METHODS_METHODS_HPP_
#define METHODS_METHODS_HPP_

#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include "topological_sorting.hpp"

namespace graph {

/**
 * @brief Функция-помощник.
 *
 * @param input входной json-файл.
 * @param output выходгой json-файл.
 * @param type тип графа.
 */

template<class T>
static int TopologicalSortingMethodHelper(const nlohmann::json& input,
                                     nlohmann::json* output,
                                     std::string type);

int TopologicalSortingMethod(const nlohmann::json& input,
                             nlohmann::json* output) {
  /*
  С классом nlohmann::json можно работать как со словарём.
  Метод at() в отличие оператора [] не меняет объект, поэтому
  этот метод можно использовать с константными объектами.
  */
  std::string type = input.at("type");

  /* Пока реализована только поддержка обычных ориентированных графов */
  if (type == "oriented_graph") {
    return TopologicalSortingMethodHelper<OrientedGraph>(input, output, type);
}
    return -1;
}

/**
 * @brief Метод топологической сортировки.
 *
 * @tparam T Тип графа.
 *
 * @param input Входные данные в формате JSON.
 * @param output Выходные данные в формате JSON.
 * @param type Строковое представление типа данных мортируемых элементов.
 * @return Функция возвращает 0 в случае успеха и отрицательное число
 * если входные данные заданы некорректно.
 *
 * Функция запускает алгоритм топологической сортировки, используя входные данные
 * в JSON формате. Результат также выдаётся в JSON формате. Функция
 * используется для сокращения кода, необходимого для поддержки различных типов данных.
 */

template<class T>
static int TopologicalSortingMethodHelper(const nlohmann::json& input,
                                     nlohmann::json* output,
                                     std::string type) {
  (*output)["id"] = input.at("id");
  (*output)["type"] = type;

  size_t size = input.at("size");

  size_t numEdges = input.at("numEdges");

  T oriented_graph;

  for (size_t i = 0; i < size; i++) {
    /* Для словарей используется индекс в виде строки,
    а для массивов просто целое число типа size_t. */
    oriented_graph.AddVertex(input.at("vertices").at(i));
  }


  for (size_t i = 0; i < numEdges; i++) {
    /* Для словарей используется индекс в виде строки,
    а для массивов просто целое число типа size_t. */
    oriented_graph.AddEdge(input.at("edges").at(i).at(0),
                           input.at("edges").at(i).at(1));
  }

  std::vector<size_t> result;

  TopologicalSorting(oriented_graph, &result);

  /* Сохраняем в ответе результат работы алгоритма. */
  (*output)["size"] = size;

  for (size_t i = 0; i < result.size(); i++) {
    (*output)["simpledata"][i] = result[i];
    (*output)["randomdata"][i][0] = result[i];
    (*output)["randomdata"][i][1] = i + 1;
  }
  return 0;
}

}  // namespace graph

#endif  // METHODS_METHODS_HPP_
