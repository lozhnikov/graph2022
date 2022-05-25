#include <string>
#include <nlohmann/json.hpp>
#include "maximal.hpp"
#include <cstring>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;
namespace graph {

template<typename T>
static int MaximalMethodHelper(const nlohmann::json& input,
                                     nlohmann::json* output,
                                     string type);

int MaximalMethod(const nlohmann::json& input, nlohmann::json* output) {
  /*
  С классом nlohmann::json можно работать как со словарём.
  Метод at() в отличие оператора [] не меняет объект, поэтому
  этот метод можно использовать с константными объектами.
  */
  string type = input.at("type");

  /* Пока реализована только поддержка целых чисел и чисел типа double. */
  if (type == "int") {
    return MaximalMethodHelper<int>(input, output, type);
  } else if (type == "float") {
    return MaximalMethodHelper<float>(input, output, type);
  } else if (type == "double") {
    return MaximalMethodHelper<double>(input, output, type);
  } else if (type == "long double") {
    return MaximalMethodHelper<long double>(input, output, type);
  }

  return -1;
}

/**
 * @brief Метод Алгоритм Крускала.
 *
 * @tparam T Тип данных сортируемых элементов.
 *
 * @param input Входные данные в формате JSON.
 * @param output Выходные данные в формате JSON.
 * @param type Строковое представление типа данных мортируемых элементов.
 * @return Функция возвращает 0 в случае успеха и отрицательное число
 * если входные данные заданы некорректно.
 *
 * Функция запускает Алгоритм Крускала, используя входные данные
 * в JSON формате. Результат также выдаётся в JSON формате. Функция
 * используется для сокращения кода, необходимого для поддержки различных типов данных.
 */
template<typename T>
static int MaximalMethodHelper(const nlohmann::json& input,
                                     nlohmann::json* output,
                                     string type) {
  (*output)["id"] = input.at("id");
  (*output)["type"] = type;

  size_t size = input.at("size").at(0);
  size_t n = input.at("size").at(1);
  if (n*size == 0) {
    (*output)["size"] = 0;
    (*output)["data"] = nullptr;
      return 0;
  }

  T* data = new T[size*3];

  for (size_t i = 0; i < size*3; i++) {
    /* Для словарей используется индекс в виде строки,
    а для массивов просто целое число типа size_t. */
    data[i] = input.at("data").at(i);
  }

  /* Здесь вызывается сам алгоритм сортировки вставками. */
  int* ostov = Maximal(data, size, n);

  /* Сохраняем в ответе результат работы алгоритма. */
  (*output)["size"] = n-1;


  for (size_t i = 0; i < (n-1)*2; i++)
    (*output)["data"][i] = ostov[i];

  delete[] data;
  delete[] ostov;

  return 0;
}

}  // namespace graph
