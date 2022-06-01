/**
 * @file tests/test.hpp
 * @author Eugene Yakimov
 *
 * Заголовочный файл для объявлений основных тестовых функций.
 */

#ifndef TESTS_TEST_HPP_
#define TESTS_TEST_HPP_

#include <httplib.h>

/**
 * @brief Набор тестов для класса graph::Graph.
 */
void TestGraph();

/**
 * @brief Набор тестов для класса graph::OrientedGraph.
 */
void TestOrientedGraph();

/**
 * @brief Набор тестов для класса graph::WeightedGraph.
 */
void TestWeightedGraph();

/**
 * @brief Набор тестов для функции graph::WeightedOrientedGraph().
 */
void TestWeightedOrientedGraph();

/* Сюда нужно добавить объявления тестовых функций. */

/**
 * @brief Набор тестов для алгоритма сортировки вставками.
 *
 * @param cli Указатель на HTTP клиент.
 */
void TestTopologicalSorting(httplib::Client* cli);

/** 
 * @brief функции тесты для соответствующих алгоритмов.
 *
 * @param cli Указатель на HTTP клиент.
 */
void TestCutPoints(httplib::Client* cli);
void TestFindBridges(httplib::Client* cli);
void TestMaximal(httplib::Client* cli);

/* Конец вставки. */

#endif  // TESTS_TEST_HPP_
