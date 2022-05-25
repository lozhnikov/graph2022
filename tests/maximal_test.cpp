#include <maximal.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include "test_core.hpp"
#include "test.hpp"
#include <bits/stdc++.h>

static void SimpleTest(httplib::Client* cli);
static void RandomTest(httplib::Client* cli);

static void RandomIntegerHelperTest(httplib::Client* cli, std::string type);


void TestMaximal(httplib::Client* cli) {
  TestSuite suite("TestMaximal");

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
    "size": [ 0, 5 ],
    "data": [ ]
  }
)"_json;

    /* Делаем POST запрос по адресу нашего метода на сервере.
    Метод dump() используется для преобразования JSON обратно в строку.
    (Можно было сразу строку передать). При передаче JSON данных
    необходимо поставить тип MIME "application/json".
    */
    httplib::Result res1 = cli->Post("/Maximal", input1.dump(),
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
    "id": 1,
    "type": "int",
    "size": [ 5, 5 ],
    "data": [ 1, 0, 1, 2, 1, 2, 3, 3, 2, 4, 3, 4, 1, 4, 2 ]
  }
)"_json;
    httplib::Result res = cli->Post("/Maximal", input.dump(),
        "application/json");

    /* Используем метод parse() для преобразования строки ответа сервера
    (res->body) в объект JSON. */
    nlohmann::json output = nlohmann::json::parse(res->body);

    /* Проверка результатов сортировки. */
    REQUIRE_EQUAL(4, output["size"]);    // edges
    REQUIRE_EQUAL(1, output["id"]);
    REQUIRE_EQUAL("int", output["type"]);
    int req[5][5];
    for (size_t i = 0; i < 5; i++)
      for (size_t j = 0; j < 5; j++)
        req[i][j] = 0;
    req[0][1] = 1;
    req[1][0] = 1;
    req[1][2] = 1;
    req[2][1] = 1;
    req[2][3] = 1;
    req[2][4] = 1;
    req[4][2] = 1;
    req[3][2] = 1;
    int req2[5][5];
    for (size_t i = 0; i < 5; i++)
      for (size_t j = 0; j < 5; j++)
        req2[i][j] = 0;
    for (size_t i = 0; i < 4; i++) {
      req2[static_cast<size_t>(output["data"][i*2])]
        [static_cast<size_t>(output["data"][i*2+1])] = 1;
      req2[static_cast<size_t>(output["data"][i*2+1])]
        [static_cast<size_t>(output["data"][i*2])] = 1;
    }
    for (size_t i = 0; i < 5; i++)
      for (size_t j = 0; j < 5; j++)
        REQUIRE_EQUAL(req[i][j], req2[i][j]);
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
 * @tparam T Тип данных сортируемых элементов.
 *
 * @param cli Указатель на HTTP клиент.
 * @param type Строковое представление типа данных сортируемых элементов.
 *
 * Функция используется для сокращения кода, необходимого для поддержки
 * различных типов данных.
 */
static void RandomIntegerHelperTest(httplib::Client* cli, std::string type) {
  // Число попыток.
  const int numTries = 100;
  // Используется для инициализации генератора случайных чисел.
  std::random_device rd;
  // Генератор случайных чисел.
  std::mt19937 gen(rd());
  // Распределение для количества элементов массива.
  std::uniform_int_distribution<size_t> arraySize(10, 100);
  // Распределение для элементов массива.
  std::uniform_int_distribution<int> elem(-10'000, 10'000);
  std::uniform_int_distribution<size_t> yesno(0, 1);

  for (int it = 0; it < numTries; it++) {
    // Получаем случайный размер массива, используя функцию распределения.
    size_t n = arraySize(gen);
    nlohmann::json input;
    size_t* matrix = new size_t[n*n];
    for (size_t i = 0; i < n; i++)
      for (size_t j = i+1; j < n; j++) {
        if (yesno(gen)) {
          matrix[i*n+j] = 1;
        } else {
          matrix[i*n+j] = 0;
        }
      }
    size_t size = 0;
    int* cost = new int[n*n];
    for (size_t i = 0; i < n; i++)
      for (size_t j = i+1; j < n; j++) {
        if (matrix[i*n+j] == 1) {
          cost[i*n+j] = elem(gen);
          input["data"][size*3] = cost[i*n+j];
          input["data"][size*3+1] = i;
          input["data"][size*3+2] = j;
          size++;
        } else {
          cost[i*n+j] = 0;
        }
      }
      // Записываем элемент в JSON.
    input["id"] = it;
    input["type"] = type;
    input["size"][0] = size;
    input["size"][1] = n;

    // Отправляем данные на сервер POST запросом.
    httplib::Result res = cli->Post("/Maximal", input.dump(),
        "application/json");

    // Используем метод parse() для преобразования строки ответа сервера
    // (res->body) в объект JSON.
    nlohmann::json output = nlohmann::json::parse(res->body);

    // Проверка результатов сортировки.
    REQUIRE_EQUAL(n-1, output["size"]);
    REQUIRE_EQUAL(it, output["id"]);
    REQUIRE_EQUAL(type, output["type"]);
  }
}



