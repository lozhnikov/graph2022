/**
 * @file tests/find_max_matching_test.cpp
 * @author Talpa Grigory
 *
 * Реализация набора тестов для метода поиска наибольшего паросочетания.
 */

#include <find_max_matching.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include "test_core.hpp"
#include "test.hpp"

static void SimpleTest(httplib::Client* cli);
static void RandomTest(httplib::Client* cli);

void TestFindMaxMatching(httplib::Client* cli) {
  TestSuite suite("TestFindMaxMatching");

  RUN_TEST_REMOTE(suite, cli, SimpleTest);
  RUN_TEST_REMOTE(suite, cli, RandomTest);
}

/**
 * @brief Простейший статический тест.
 *
 * @param cli Указатель на HTTP клиент.
 */

static void SimpleTest(httplib::Client* cli) {
  {
    /*
     Библиотека nlohmann json позволяет преобразовать
     строку в объект nlohmann::json не только при помощи
     функции nlohmann::json::parse(), но и при помощи
     специального литерала _json. Если его поставить после строки
     в кавычках, то она конвертируется в json объект.
     R"(
     )" Так записывается строка, содержащая символы перевода строки
     в C++. Всё, что между скобками это символы строки. Перводы строк
     можно ставить просто как перевод строки в текстовом редактора
     (а не через \n).
    */  
      nlohmann::json input = R"(
  {
    "id": 1,
    "type": "graph",
    "size": 4,
    "vertices": [ 1, 2, 3, 4],
    "numEdges": 3,
    "edges": [[1,2],[2,3],[3,4]]
  }
)"_json;
      /* Делаем POST запрос по адресу нашего метода на сервере.
    Метод dump() используется для преобразования JSON обратно в строку.
    (Можно было сразу строку передать). При передаче JSON данных
    необходимо поставить тип MIME "application/json".
    */
    httplib::Result res = cli->Post("/FindMaxMatching", input.dump(),
        "application/json");
      /* Используем метод parse() для преобразования строки ответа сервера
    (res->body) в объект JSON. */ 
    nlohmann::json output = nlohmann::json::parse(res->body);
    REQUIRE_EQUAL(4, output["size"]);
    REQUIRE_EQUAL(1, output["id"]);
    REQUIRE_EQUAL("graph", output["type"]);
    REQUIRE_EQUAL(output["data"].size(), static_cast<size_t>(2));
      /*Второй тест*/
    nlohmann::json input1 = R"(
  {
    "id": 2,
    "type": "graph",
    "size": 2,
    "vertices": [ 2 , 1 ],
    "numEdges": 1,
    "edges": [[1,2]]
  }
)"_json;

    httplib::Result res1 = cli->Post("/FindMaxMatching", input1.dump(),
        "application/json");
    nlohmann::json output1 = nlohmann::json::parse(res1->body);

    REQUIRE_EQUAL(2, output1["size"]);
    REQUIRE_EQUAL(2, output1["id"]);
    REQUIRE_EQUAL("graph", output1["type"]);
    REQUIRE_EQUAL(output1["data"].size(), static_cast<size_t>(1));
       /*Третий тест*/
    nlohmann::json input2 = R"(
  {
    "id": 3,
    "type": "graph",
    "size": 6,
    "vertices": [ 6,4,5,2, 1,3 ],
    "numEdges": 5,
    "edges": [[1,3],[2,3],[3,4],[4,5],[4,6]]
  }
)"_json;

    httplib::Result res2 = cli->Post("/FindMaxMatching", input2.dump(),
        "application/json");
    nlohmann::json output2 = nlohmann::json::parse(res2->body);
    REQUIRE_EQUAL(6, output2["size"]);
    REQUIRE_EQUAL(3, output2["id"]);
    REQUIRE_EQUAL("graph", output2["type"]);
    REQUIRE_EQUAL(output2["data"].size(), static_cast<size_t>(2));
}
}

/**
 * @brief Простейший случайный тест для целых чисел.
 *
 *
 * @param cli Указатель на HTTP клиент.
 *
 * Функция используется для сокращения кода, необходимого для поддержки
 * различных типов данных.
 */
  static void RandomTest(httplib::Client* cli) {
  // Число попыток.
  const int numTries = 100;
  // Используется для инициализации генератора случайных чисел.
  std::random_device rd;
  // Генератор случайных чисел.
  std::mt19937 gen(rd());
  // Распределение для количества элементов массива.
  std::uniform_int_distribution<size_t> arraySize(10, 50);

  for (int it = 0; it < numTries; it++) {
    // Получаем случайный размер массива, используя функцию распределения.
    size_t size = arraySize(gen);
    // Случайный номер вершины
    std::uniform_int_distribution<size_t> vert(1, size-4);
    nlohmann::json input;

    input["id"] = it;
    input["type"] = "graph";
    input["size"] = size;
    size_t numEdges = size-4;
    input["numEdges"] = numEdges;


    for (size_t i = 0; i < size; i++) {
      // Записываем элемент в JSON.
      input["vertices"][i] = i+1;
    }
    // Генирируем ребра случайный образом.
    for (size_t i = 0; i < numEdges-3; i++) {
        input["edges"][i][0] = vert(gen);
        input["edges"][i][1] = vert(gen);
    }
    // Добавляем отдельную цепь из 3 ребер
    input["edges"][numEdges-3][0] =  size - 3;
    input["edges"][numEdges-3][1] = size-2;
    input["edges"][numEdges-2][0] =  size - 2;
    input["edges"][numEdges-2][1] = size-1;
    input["edges"][numEdges - 1][0] =  size - 1;
    input["edges"][numEdges - 1][1] = size;

    /* Отправляем данные на сервер POST запросом. */
     httplib::Result res = cli->Post("/FindMaxMatching", input.dump(),
        "application/json");

    /* Используем метод parse() для преобразования строки ответа сервера
    (res->body) в объект JSON. */
    nlohmann::json output = nlohmann::json::parse(res->body);

    /* Проверка результатов метода. */

    bool find1 = false;
    bool find2 = false;
    size_t i = 0;
     while ((i < output["data"].size())&&(!find1)) {
       find1 = ((output["data"].at(i).at(0) == size - 3)&&
               (output["data"].at(i).at(1) == size-2)) ||
           ((output["data"].at(i).at(1) == size - 2)&&
               (output["data"].at(i).at(0) == size- 3));
           i++;
    }
    i = 0;
     while ((i < output["data"].size())&&(!find2)) {
       find2 = ((output["data"].at(i).at(0) == size - 1)&&
               (output["data"].at(i).at(1) == size)) ||
           ((output["data"].at(i).at(1) == size)&&
               (output["data"].at(i).at(0) == size- 1));
           i++;
    }

    REQUIRE_EQUAL(size, output["size"]);
    REQUIRE_EQUAL(it, output["id"]);
    REQUIRE_EQUAL("graph", output["type"]);
    // Парасочетаний не может быть больше, чем ребер.
    REQUIRE_EQUAL(true, output["data"].size() <= numEdges);
    // 2 не смежных ребра из добаленой цепи должны быть в паросочетании.
    REQUIRE_EQUAL(true, (find1 && find2));
  }
}
