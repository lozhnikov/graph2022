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

T* data = new T[size*3];

for (size_t i = 0; i < size*3; i++) {
/* Для словарей используется индекс в виде строки,
а для массивов просто целое число типа size_t. */
data[i] = input.at("data").at(i);
}


/* Здесь вызывается сам алгоритм Беллмана-Форда. */
std::vector<T> res = Belmanf(data, size, n, v);

/* Сохраняем в ответе результат работы алгоритма. */
(*output)["size"] = n;

for (size_t i = 0; i < n; i++)
(*output)["data"][i] = res[i];

delete[] data;

return 0;
}

}   // namespace graph

