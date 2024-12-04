#include <array>
#include <iostream>
#include <print>
#include <vector>

using namespace std;

array<vector<int>, 2> read_input() {
  // TODO: This can be made polymorphic over multiple outputs
  array<vector<int>, 2> result;
  for (string line; getline(cin, line) && !line.empty();) {
  }
  return result;
}

int main() {}