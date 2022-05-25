#ifndef INCLUDE_MAXIMAL_HPP_
#define INCLUDE_MAXIMAL_HPP_

#include <unordered_map>
#include <unordered_set>
#include <cstddef>
#include <cstring>
#include <utility>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <iterators.hpp>
using namespace std;

namespace graph {
template<typename T>
int* Maximal(T* data, size_t m, size_t n) {
  vector < pair < T, pair < size_t, size_t > > > g(m);
  // вес - вершина 1 - вершина 2
  for (size_t k = 0; k < m; k++) {
    g[k].first = data[k*3];
    g[k].second.first = static_cast<size_t>(data[k*3+1]);
    g[k].second.second = static_cast<size_t>(data[k*3+2]);
  }
  T cost = 0;
  vector <pair<int, int>> res;

  sort(g.begin(), g.end());
  vector<size_t> tree_id(n);
  for (size_t i = 0; i < n; ++i)
    tree_id[i] = i;
  for (size_t i = 0; i < m; ++i) {
    size_t a = g[i].second.first,  b = g[i].second.second;
    T l = g[i].first;
    if (tree_id[a] != tree_id[b]) {
      cost += l;
      res.push_back(make_pair(a, b));
      size_t old_id = tree_id[b],  new_id = tree_id[a];
      for (size_t j = 0; j < n; ++j)
        if (tree_id[j] == old_id)
          tree_id[j] = new_id;
    }
  }
  if (res.size() != n-1) {
    ostringstream s;
    s << "wrong graph";
    throw runtime_error(s.str());
  }
  int* ostov = new int[res.size()*2];
  for (size_t ll = 0; ll < res.size(); ll++) {
    ostov[ll*2] = res[ll].first;
    ostov[ll*2+1] = res[ll].second;
  }
  return ostov;
}

}   // namespace graph

#endif  // INCLUDE_MAXIMAL_HPP_
