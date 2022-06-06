/**
 * @file methods/ingeborgadapkunaite_method.cpp
 * @author Nikita Gordeev
 *
 * Реализация серверной части метода.
 */

#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include "ingeborgadapkunaite.hpp"
#include "iterators.hpp"
namespace graph {

/**
 * @brief Функция-помошник. Вызывает необходимый метод в зависимости от типа 
 графа.
 *
 * @param input входной json-файл.
 * @param input выходгой json-файл.
 * @param type тип графа.
 */

template<typename T>
static int IngeborgadapkunaiteMethodHelper(const nlohmann::json& input,
                                     nlohmann::json* output,
                                     std::string type);

int IngeborgadapkunaiteMethod(const nlohmann::json& input, nlohmann::json* output) {
  /*
  С классом nlohmann::json можно работать как со словарём.
  Метод at() в отличие оператора [] не меняет объект, поэтому
  этот метод можно использовать с константными объектами.
  */
  std::string type = input.at("type");

  /* Пока реализована только поддержка целых чисел и чисел типа double. */
  if (type == "int") {
    return IngeborgadapkunaiteMethodHelper<int>(input, output, type);
  } else if (type == "float") {
    return IngeborgadapkunaiteMethodHelper<float>(input, output, type);
  } else if (type == "double") {
    return IngeborgadapkunaiteMethodHelper<double>(input, output, type);
  } else if (type == "long double") {
    return IngeborgadapkunaiteMethodHelper<long double>(input, output, type);
  }

  return -1;
}

/**
 * @brief Метод Алгоритм Куна.
 *
 * @param input Входные данные в формате JSON.
 * @param output Выходные данные в формате JSON.
 * @param type Строковое представление типа данных мортируемых элементов.
 * @return Функция возвращает 0 в случае успеха и отрицательное число
 * если входные данные заданы некорректно.
 *
 * Функция запускает Алгоритм Куна, используя входные данные
 * в JSON формате. Результат также выдаётся в JSON формате. Функция
 * используется для сокращения кода, необходимого для поддержки различных 
 * типов данных.
 */
template<typename T>
static int IngeborgadapkunaiteMethodHelper(const nlohmann::json& input,
                                     nlohmann::json* output,
                                     std::string type) {
  (*output)["id"] = input.at("id");
  (*output)["type"] = type;

  size_t size = input.at("size").at(0);
  size_t n = input.at("size").at(1);
  if (n*size == 0) {
    (*output)["size"] = 0;
    (*output)["data"] = nullptr;
      return 0;
  }

  /* Для словарей используется индекс в виде строки,
    а для массивов просто целое число типа size_t. */
  Graph Tolstoi;
  for (size_t i = 0; i < size; i++)
    Tolstoi.AddEdge(static_cast<size_t>(input.at("data").at(i*2)), 
                   static_cast<size_t>(input.at("data").at(i*2+1))); 

  /* Здесь вызывается сам алгоритм Куна. */
  Graph Orlov = Ingeborgadapkunaite(Tolstoi);
  std::vector <std::pair<size_t, size_t>> edges;
  for (size_t v : Orlov.Vertices()) {
    for (size_t neighbour : Orlov.Edges(v)) {
      edges.push_back({v, neighbour});
    }
  }

  /* Сохраняем в ответе результат работы алгоритма. */
  (*output)["size"] = edges.size();


  for (size_t i = 0; i < edges.size(); i++)
    (*output)["data"][i] = edges[i];

  return 0;
}

}  // namespace graph