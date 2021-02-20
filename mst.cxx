// vim: sw=4 ts=4 expandtab
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <map>
#include <mstch/mstch.hpp>

int main(int argc, char *argv[]) {
    std::ifstream tpl_fstream{ argv[1] };
    std::string tpl{ std::istreambuf_iterator<char>{ tpl_fstream },
                     std::istreambuf_iterator<char>{} };
    mstch::map ctx{};

    for(int i=2; i+1<argc; i+=2) {
        ctx.emplace(std::make_pair(std::string(argv[i]), std::string(argv[i+1])));
    }

    std::cout << mstch::render(tpl, ctx);
    return 0;
}

