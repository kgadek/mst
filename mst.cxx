// vim: sw=4 ts=4 expandtab
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <map>
#include <cstdio>
#include <mstch/mstch.hpp>

int main(int argc, char *argv[]) {
    mstch::config::escape = [](const std::string& str) -> std::string {
        return str;
    };
    std::ifstream tpl_fstream{ argv[1] };
    std::string tpl{ std::istreambuf_iterator<char>{ tpl_fstream },
                     std::istreambuf_iterator<char>{} };
    mstch::map ctx{
        {"include", mstch::lambda{ [](const std::string &filename) -> mstch::node {
            std::ifstream file_fstream{ filename };
            return std::string { std::istreambuf_iterator<char>{ file_fstream },
                                 std::istreambuf_iterator<char>{} };
        }}},
        {"env", mstch::lambda{ [](const std::string &env_var_name) -> mstch::node {
            if(char *env_var_value = std::getenv(env_var_name.c_str())) {
                return std::string{env_var_value};
            } else {
                return std::string{};
            }
        }}}
    };

    for(int i=2; i+1<argc; i+=2) {
        ctx.emplace(std::make_pair(std::string(argv[i]), std::string(argv[i+1])));
    }

    std::cout << mstch::render(tpl, ctx);
    return 0;
}

