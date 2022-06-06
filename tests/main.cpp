/**
 * @file tests/main.cpp
 * @author Vladislav Zyuzin
 *
 * Файл с функией main() для клиентской части программы (набора тестов).
 */

#include <httplib.h>
#include "test.hpp"
#include "test_core.hpp"


int main(int argc, char* argv[]) {
  // Порт по-умолчанию.
  int port = 2022;
  // Адрес сервера по-умолчанию.
  std::string host = "127.0.0.1";

  // Обычные тесты на сборку для базовых шаблонов.
  // Сюда ничего добавлять не нужно.
  TestGraph();
  TestOrientedGraph();
  TestWeightedGraph();
  TestWeightedOrientedGraph();

  if (argc >= 2) {
    // Меняем хост, если предоставлен соответствующий аргумент командной строки.
    host = std::string(argv[1]);
  }

  if (argc >= 3) {
    // Меняем порт, если предоставлен соответствующий аргумент командной строки.
    if (std::sscanf(argv[2], "%d", &port) != 1)
      return -1;
  }

  // Адрес подключения в формате хост:порт.
  std::string url = host + ":" + std::to_string(port);

  httplib::Client cli(url.c_str());

  /* Сюда нужно вставить вызов набора тестов для алгоритма. */

  TestTopologicalSorting(&cli);
  TestCutPoints(&cli);
    TestFindBridges(&cli);
  TestMaximal(&cli);
  TestNegCycle(&cli);
  TestBelmanf(&cli);

  /* Конец вставки. */

  // Отправляем GET запрос для остановки сервера.
  httplib::Result res = cli.Get("/stop");

  if (res->status != 200)
    return -1;

  return TestSuite::Status();
}
