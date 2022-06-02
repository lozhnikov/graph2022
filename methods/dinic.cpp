/**
 * @file methods/find_bridges.cpp
 * @author Denis Maslenikov
 *
 * Реализация серверной части метода.
 */
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include "dinic.hpp"

namespace graph {
/**
 * @brief Функция-помошник. Вызывает необходимый метод в зависимости от типа 
 графа.
 *
 * @param input входной json-файл.
 * @param input выходгой json-файл.
 * @param type тип графа.
 */
template<class T>
static int DinicMethodHelper(const nlohmann::json& input,
                                     nlohmann::json* output,
                                     std::string type);

int DinicMethod(const nlohmann::json& input, nlohmann::json* output) {
  std::string type = input.at("type");

  if (type == "weighted_oriented_graph")
    return graph::DinicMethodHelper<graph::WeightedOrientedGraph<int>>(input, output, type);
  return -1;
}

/**
 * @brief Метод Диница.
 *
 * @tparam T Тип графа.
 *
 * @param input Входные данные в формате JSON.
 * @param output Выходные данные в формате JSON.
 * @param type Строковое представление типа данных мортируемых элементов.
 *
 * Функция запускает алгоритм поиска мостов, используя входные данные
 * в JSON формате. Результат также выдаётся в JSON формате. Функция
 * используется для сокращения кода, необходимого для поддержки различных типов данных.
 */
template<class T>
static int DinicMethodHelper(const nlohmann::json& input,
                                     nlohmann::json* output,
                                     std::string type) {
  (*output)["id"] = input.at("id");
  (*output)["type"] = type;

  const size_t size = input.at("size");
  const size_t numEdges = input.at("numEdges");
  const size_t s = input.at("start");
  const size_t t = input.at("theEnd");

  T graph;
  for (size_t i = 0; i < size; i++) {

    graph.AddVertex(input.at("vertices").at(i));
  }
  for (size_t i = 0; i < numEdges; i++) {
    /*
    printf("DEBUG: add(%zu, %zu, %zu)\n",
		    static_cast<size_t>(input.at("edges").at(i).at(0)),
		    static_cast<size_t>(input.at("edges").at(i).at(1)),
		    static_cast<size_t>(input.at("edges").at(i).at(2)));
    */
    graph.AddEdge(input.at("edges").at(i).at(0), input.at("edges").at(i).at(1),
		    input.at("edges").at(i).at(2));
    //printf("DEBUG: size=%d\n", graph.Edges(1).size());
  }

  T result;
  /* вызов алгоритма */
  Dinic<int>(graph, result, s, t);

  (*output)["size"] = result.NumVertices();
  size_t resEdges = 0;
  for (size_t v : result.Vertices())
	  resEdges += result.Edges(v).size();
  (*output)["numEdges"] = resEdges;
  size_t i = 0;
  for (size_t v : result.Vertices())
	  (*output)["vertices"][i++] = v;
  size_t j = 0;
  for (size_t v : result.Vertices())
	  for (size_t e : result.Edges(v)) {
		  (*output)["edges"][j][0] = v;
		  (*output)["edges"][j][1] = e;
		  (*output)["edges"][j][2] = result.EdgeWeight(v, e);
		  j++;
	  }

  return 0;
}

}// namespace graph
