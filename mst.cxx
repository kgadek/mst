#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <map>
#include <mstch/mstch.hpp>

int main(int argc, char *argv[]) {
  std::ifstream tpl_file{argv[1]};
  std::string tpl{std::istreambuf_iterator<char>(tpl_file), std::istreambuf_iterator<char>()};
  mstch::map ctx{};

  for(int i=2; i<argc; i+=2) {
    ctx.emplace(std::make_pair(std::string(argv[i]), std::string(argv[i+1])));
  }

  std::cout << mstch::render(tpl, ctx);
  return 0;
}

