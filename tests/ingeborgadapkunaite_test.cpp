/**
 * @file tests/ingeborgadapkunaite_test.cpp
 * @author Nikita Gordeev
 *
 * Реализация набора тестов для Алгоритм Куна нахождения наибольшего
 * паросочетания в двудольном графе.
 */

#include <ingeborgadapkunaite.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include "test_core.hpp"
#include "test.hpp"

static void SimpleTest(httplib::Client* cli);
static void RandomTest(httplib::Client* cli);

static void RandomIntegerHelperTest(httplib::Client* cli, std::string type);


void TestIngeborgadapkunaite(httplib::Client* cli) {
  TestSuite suite("TestIngeborgadapkunaite");

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
    nlohmann::json input1 = R"(
  {
    "id": 1,
    "type": "int",
    "size": [ 0, 11 ],
    "data": [ ]
  }
)"_json;

    /* Делаем POST запрос по адресу нашего метода на сервере.
    Метод dump() используется для преобразования JSON обратно в строку.
    (Можно было сразу строку передать). При передаче JSON данных
    необходимо поставить тип MIME "application/json".
    */
    httplib::Result res1 = cli->Post("/Ingeborgadapkunaite", input1.dump(),
        "application/json");

    /* Используем метод parse() для преобразования строки ответа сервера
    (res1->body) в объект JSON. */
    nlohmann::json output1 = nlohmann::json::parse(res1->body);

    /* Проверка результатов сортировки. */
    REQUIRE_EQUAL(0, output1["size"]);    // edges
    REQUIRE_EQUAL(1, output1["id"]);
    REQUIRE_EQUAL("int", output1["type"]);
    REQUIRE_EQUAL(nullptr, output1["data"]);



    nlohmann::json input = R"(
  {
    "id": 2,
    "type": "int",
    "size": [ 1, 100 ],
    "data": [ 101, 1001 ]
  }
)"_json;
    httplib::Result res = cli->Post("/Ingeborgadapkunaite", input.dump(),
        "application/json");

    /* Используем метод parse() для преобразования строки ответа сервера
    (res->body) в объект JSON. */
    nlohmann::json output = nlohmann::json::parse(res->body);

    /* Проверка результатов сортировки. */
    bool flag = (((output["data"][0][0] == 101) &&
                  (output["data"][0][1] == 1001)) ||
                 ((output["data"][0][1] == 101) &&
                  (output["data"][0][0] == 1001)));
    REQUIRE_EQUAL(flag, 1);
    REQUIRE_EQUAL(2, output["size"]);    // edges
    REQUIRE_EQUAL(2, output["id"]);
    REQUIRE_EQUAL("int", output["type"]);
    nlohmann::json input3 = R"(
  {
    "id": 3,
    "type": "int",
    "size": [ 7, 10],
    "data": [ 2, 10, 17, 11, 15, 3, 7, 4, 8, 6, 8, 4, 15, 11 ]
  }
)"_json;
    httplib::Result res3 = cli->Post("/Ingeborgadapkunaite", input3.dump(),
        "application/json");

    /* Используем метод parse() для преобразования строки ответа сервера
    (res3->body) в объект JSON. */
    nlohmann::json output3 = nlohmann::json::parse(res3->body);

    /* Проверка результатов сортировки. */
    REQUIRE_EQUAL(10, output3["size"]);    // edges
    REQUIRE_EQUAL(3, output3["id"]);
    REQUIRE_EQUAL("int", output3["type"]);
    int rr[18][18];
    for (size_t i = 0; i < 18; i++)
      for (size_t j = 0; j < 18; j++)
        rr[i][j] = 0;
    rr[10][2] = 1;
    rr[17][11] = 1;
    rr[15][3] = 1;
    rr[4][7] = 1;
    rr[6][8] = 1;
    rr[2][10] = 1;
    rr[11][17] = 1;
    rr[3][15] = 1;
    rr[7][4] = 1;
    rr[8][6] = 1;
    int req3[18][18];
    for (size_t i = 0; i < 18; i++)
      for (size_t j = 0; j < 18; j++)
        req3[i][j] = 0;
    for (size_t i = 0; i < 10; i++) {
      req3[static_cast<size_t>(output3["data"][i][0])]
        [static_cast<size_t>(output3["data"][i][1])] = 1;
      req3[static_cast<size_t>(output3["data"][i][1])]
        [static_cast<size_t>(output3["data"][i][0])] = 1;
    }
    for (size_t i = 0; i < 18; i++)
      for (size_t j = 0; j < 18; j++)
        REQUIRE_EQUAL(rr[i][j], req3[i][j]);
}
}
/** 
 * @brief Простейший случайный тест.
 *
 * @param cli Указатель на HTTP клиент.
 */
static void RandomTest(httplib::Client* cli) {
  RandomIntegerHelperTest(cli, "int");
}

/** 
 * @brief Простейший случайный тест для целых чисел.
 *
 * @param cli Указатель на HTTP клиент.
 * @param type Строковое представление типа данных сортируемых элементов.
 *
 * Функция используется для сокращения кода, необходимого для поддержки
 * различных типов данных.
 */
static void RandomIntegerHelperTest(httplib::Client* cli, std::string type) {
  // Число попыток.
  const int numTries = 10;
  // Используется для инициализации генератора случайных чисел.
  std::random_device rd;
  // Генератор случайных чисел.
  std::mt19937 gen(rd());
  // Распределение для количества элементов массива.
  std::uniform_int_distribution<size_t> arraySize(10, 20);
  // Распределение для элементов массива.
  std::uniform_int_distribution<size_t> yesno(0, 1);

  for (int it = 0; it < numTries; it++) {
    // Получаем случайный размер массива, используя функцию распределения.
    size_t n = arraySize(gen);
    nlohmann::json input;
    size_t size = 0;
    for (size_t i = 2; i < n; i++)
      for (size_t j = i+1; j < n; j++) {
        if (yesno(gen)) {
          input["data"][size*2] = 2*i;
          input["data"][size*2+1] = 2*j+1;
          size++;
        }
      }
      // Записываем элемент в JSON.
    input["id"] = it;
    input["type"] = type;
    input["size"][0] = size;
    input["size"][1] = n;

    // Отправляем данные на сервер POST запросом.
    httplib::Result res = cli->Post("/Ingeborgadapkunaite", input.dump(),
        "application/json");

    // Используем метод parse() для преобразования строки ответа сервера
    // (res->body) в объект JSON.
    nlohmann::json output = nlohmann::json::parse(res->body);

    // Проверка результатов сортировки.
    REQUIRE_EQUAL(it, output["id"]);
    REQUIRE_EQUAL(type, output["type"]);
    graph::Graph inputGraph, outputGraph;
    for (size_t i = 0; i < size; i++)
      inputGraph.AddEdge(static_cast<size_t>(input["data"][i*2]),
                         static_cast<size_t>(input["data"][i*2+1]));
    size_t s = output["size"];
    for (size_t i = 0; i < s; i++)
      outputGraph.AddEdge(static_cast<size_t>(output["data"][i][0]),
                         static_cast<size_t>(output["data"][i][1]));
    for (size_t v : outputGraph.Vertices()) {
      int count = 0;
      for (size_t neighbour : outputGraph.Edges(v)) {
        count++;
        neighbour++;
        neighbour--;
      }
      REQUIRE_EQUAL(1, count);
    }
    s /= 2;

    if (s != size) {
      for (size_t v : inputGraph.Vertices())
        for (size_t neighbour : inputGraph.Edges(v))
          if (!outputGraph.HasEdge(v, neighbour)) {
            outputGraph.AddEdge(v, neighbour);
            neighbour++;
            neighbour--;
            int flag = 0;
            for (size_t v1 : outputGraph.Vertices()) {
              size_t c = 0;
              for (size_t neighbour1 : outputGraph.Edges(v1)) {
                c++;
                neighbour1++;
                neighbour1--;
              }
              if (c > 1)
                flag = 1;
            }
            outputGraph.RemoveEdge(v, neighbour);
            REQUIRE_EQUAL(1, flag);
          }
    }
  }
}
