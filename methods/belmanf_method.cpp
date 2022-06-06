#include <string>
#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>
#include "iterators.hpp"
#include "belmanf.hpp"
namespace graph {

template<typename T>
static int BelmanfMethodHelper(const nlohmann::json& input,
nlohmann::json* output, std::string type);

int BelmanfMethod(const nlohmann::json& input, nlohmann::json* output) {
/*
С классом nlohmann::json можно работать как со словарём.
Метод at() в отличие оператора [] не меняет объект, поэтому
этот метод можно использовать с константными объектами.
*/

  std::string type = input.at("type");

  /* Пока реализована только поддержка целых чисел и чисел типа double. */
  if (type == "int") {
    return BelmanfMethodHelper<int>(input, output, type);
  } else if (type == "float") {
    return BelmanfMethodHelper<float>(input, output, type);
  } else if (type == "double") {
    return BelmanfMethodHelper<double>(input, output, type);
  } else if (type == "long double") {
    return BelmanfMethodHelper<long double>(input, output, type);
  }

  return -1;
}

/**
* @brief Метод Беллмана-Форда.
*
* @tparam T Тип данных сортируемых элементов.
*
* @param input Входные данные в формате JSON.
* @param output Выходные данные в формате JSON.
* @param type Строковое представление типа данных мортируемых элементов.
* @return Функция возвращает 0 в случае успеха и отрицательное число
* если входные данные заданы некорректно.
*
* Функция запускает алгоритм Беллмана-Форда, используя входные данные
* в JSON формате. Результат также выдаётся в JSON формате. Функция
* используется для сокращения кода, необходимого для поддержки различных типов данных.
*/
template<typename T>
static int BelmanfMethodHelper(const nlohmann::json& input,
nlohmann::json* output, std::string type) {
  (*output)["id"] = input.at("id");
  (*output)["type"] = type;

  size_t size = input.at("size").at(0);
  size_t n = input.at("size").at(1);
  size_t v = input.at("size").at(2);
  std::vector<int> data(size*3);

  for (size_t i = 0; i < size*3; i++) {
  /* Для словарей используется индекс в виде строки,
  а для массивов просто целое число типа size_t. */
    data[i] = input.at("data").at(i);
  }
  if (n == 0) {
    (*output)["size"] = 0;
    (*output)["data"] = "empty graph";
      return 0;
  }
  if (size == 0) {
    (*output)["size"] = n;
    (*output)["data"] = "no edges";
      return 0;
  }

/* Здесь вызывается сам алгоритм Беллмана-Форда. */
  WeightedOrientedGraph<int> wog;
  for (size_t k = 0; k < size; k++) {
      wog.AddEdge(static_cast<size_t>(data[k*3 + 1]),
                  static_cast<size_t>(data[k*3 + 2]), data[k*3]);
    }
  std::unordered_map<size_t , std::pair<bool, int>> res = Belmanf(wog, v);

/* Сохраняем в ответе результат работы алгоритма. */
  (*output)["size"] = n;
  size_t j = 0;
  for (auto it = res.begin(); it != res.end(); it++) {  // выводим их
    if ((it->second).first == 1) {
      size_t tmp1 = it->first;
      int tmp2 = (it->second).second;
      std::string s2 = std::to_string(tmp2);
      (*output)["data"][j][0] = tmp1;
      (*output)["data"][j][1] = s2;
    } else {
      size_t tmp1 = it->first;
      (*output)["data"][j][0] = tmp1;
      (*output)["data"][j][1] = "no";
    }
    j++;
  }

  return 0;
}

}   // namespace graph

