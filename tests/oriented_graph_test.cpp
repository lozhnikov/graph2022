/**
 * @file oriented_graph_test.cpp
 * @author Mikhail Lozhnikov
 *
 * Тесты для класса graph::OrientedGraph.
 */

#include "test_core.hpp"
#include <oriented_graph.hpp>

using graph::OrientedGraph;

static void SimpleTest();
static void AnotherSimpleTest();

void TestOrientedGraph() {
  TestSuite suite("TestOrientedGraph");

  RUN_TEST(suite, SimpleTest);
  RUN_TEST(suite, AnotherSimpleTest);
}

static void SimpleTest() {
  OrientedGraph graph;

  graph.AddVertex(1);

  REQUIRE(graph.HasVertex(1));

  graph.AddVertex(2);

  REQUIRE(graph.HasVertex(2));

  graph.AddEdge(1, 2);

  REQUIRE(graph.HasEdge(1, 2));
}

static void AnotherSimpleTest() {
  OrientedGraph graph;

  graph.AddVertex(1);

  REQUIRE(graph.HasVertex(1));

  graph.AddVertex(2);

  REQUIRE(graph.HasVertex(2));

  graph.AddEdge(1, 2);

  REQUIRE(graph.HasEdge(1, 2));

  graph.RemoveVertex(2);

  REQUIRE(!graph.HasEdge(1, 2));
}
