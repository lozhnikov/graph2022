/**
 * @file methods/main.cpp
 * @author Eugene Yakimov
 *
 * Файл с функией main() для серверной части программы.
 */

#include <httplib.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include "methods.hpp"

using graph::TopologicalSortingMethod;
using graph::CutPointsMethod;
using graph::FindBridgesMethod;
using graph::MaximalMethod;
using graph::FindMaxMatchingMethod;


int main(int argc, char* argv[]) {
  // Порт по-умолчанию.
  int port = 8080;

  if (argc >= 2) {
    // Меняем порт по умолчанию, если предоставлен соответствующий
    // аргумент командной строки.
    if (std::sscanf(argv[1], "%d", &port) != 1)
      return -1;
  }

  std::cerr << "Listening on port " << port << "..." << std::endl;

  httplib::Server svr;

  // Обработчик для GET запроса по адресу /stop. Этот обработчик
  // останавливает сервер.
  svr.Get("/stop", [&](const httplib::Request&, httplib::Response&) {
    svr.stop();
  });

  /* Сюда нужно вставить обработчик post запроса для алгоритма. */
/* /FindBridges это адрес для запросов на сортировку вставками
  на сервере. */
  svr.Post("/FindBridges", [&](const httplib::Request& req,
                                 httplib::Response& res) {
    /*
    Поле body структуры httplib::Request содержит текст запроса.
    Функция nlohmann::json::parse() используется для того,
    чтобы преобразовать текст в объект типа nlohmann::json.
    */
    nlohmann::json input = nlohmann::json::parse(req.body);
    nlohmann::json output;

    /* Если метод завершился с ошибкой, то выставляем статус 400. */
    if (FindBridgesMethod(input, &output) < 0)
      res.status = 400;

    res.set_content(output.dump(), "application/json");
  });

  /* /TopologicalSorting это адрес для запросов на топологическую сортировку
  на сервере. */  
  svr.Post("/TopologicalSorting", [&](const httplib::Request& req,
                                 httplib::Response& res) {
    /*
    Поле body структуры httplib::Request содержит текст запроса.
    Функция nlohmann::json::parse() используется для того,
    чтобы преобразовать текст в объект типа nlohmann::json.
    */
    nlohmann::json input = nlohmann::json::parse(req.body);
    nlohmann::json output;

    /* Если метод завершился с ошибкой, то выставляем статус 400. */
    if (TopologicalSortingMethod(input, &output) < 0)
      res.status = 400;

    /*
    Метод nlohmann::json::dump() используется для сериализации
    объекта типа nlohmann::json в строку. Метод set_content()
    позволяет задать содержимое ответа на запрос. Если передаются
    JSON данные, то MIME тип следует выставить application/json.
    */
    res.set_content(output.dump(), "application/json");
  });

  /* /CutPoints это адрес для запросов
  на сервере. */
  svr.Post("/CutPoints", [&](const httplib::Request& req,
                                 httplib::Response& res) {
    /*
    Поле body структуры httplib::Request содержит текст запроса.
    Функция nlohmann::json::parse() используется для того,
    чтобы преобразовать текст в объект типа nlohmann::json.
    */
    nlohmann::json input = nlohmann::json::parse(req.body);
    nlohmann::json output;

    /* Если метод завершился с ошибкой, то выставляем статус 400. */
    if (CutPointsMethod(input, &output) < 0)
      res.status = 400;

    /*
    Метод nlohmann::json::dump() используется для сериализации
    объекта типа nlohmann::json в строку. Метод set_content()
    позволяет задать содержимое ответа на запрос. Если передаются
    JSON данные, то MIME тип следует выставить application/json.
    */
    res.set_content(output.dump(), "application/json");
  });


/* /Maximal это адрес для запросов на Алгоритм Крускала. */
  svr.Post("/Maximal", [&](const httplib::Request& req1,
                                 httplib::Response& res1) {
    /*
    Поле body структуры httplib::Request содержит текст запроса.
    Функция nlohmann::json::parse() используется для того,
    чтобы преобразовать текст в объект типа nlohmann::json.
    */
    nlohmann::json input1 = nlohmann::json::parse(req1.body);
    nlohmann::json output1;

    /* Если метод завершился с ошибкой, то выставляем статус 400. */
    if (MaximalMethod(input1, &output1) < 0)
      res1.status = 400;

    /*
    Метод nlohmann::json::dump() используется для сериализации
    объекта типа nlohmann::json в строку. Метод set_content()
    позволяет задать содержимое ответа на запрос. Если передаются
    JSON данные, то MIME тип следует выставить application/json.
    */
    res1.set_content(output1.dump(), "application/json");
  });
    /* /FindMaxMatching это адрес для запросов
  на сервере. */
  svr.Post("/FindMaxMatching", [&](const httplib::Request& req,
                                 httplib::Response& res) {
    /*
    Поле body структуры httplib::Request содержит текст запроса.
    Функция nlohmann::json::parse() используется для того,
    чтобы преобразовать текст в объект типа nlohmann::json.
    */
    nlohmann::json input = nlohmann::json::parse(req.body);
    nlohmann::json output;

    /* Если метод завершился с ошибкой, то выставляем статус 400. */
    if (FindMaxMatchingMethod(input, &output) < 0)
      res.status = 400;

    /*
    Метод nlohmann::json::dump() используется для сериализации
    объекта типа nlohmann::json в строку. Метод set_content()
    позволяет задать содержимое ответа на запрос. Если передаются
    JSON данные, то MIME тип следует выставить application/json.
    */
    res.set_content(output.dump(), "application/json");
  });


  // Эта функция запускает сервер на указанном порту. Программа не завершится
  // до тех пор, пока сервер не будет остановлен.
  svr.listen("0.0.0.0", port);

  return 0;
}
