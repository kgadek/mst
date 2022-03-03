// vim: sw=4 ts=4 expandtab
#include <array>
#include <boost/exception/diagnostic_information.hpp>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <mstch/mstch.hpp>
#include <string>
#include <utility>

int main(int argc, char *argv[]) {
    try {
        mstch::config::escape = [](const std::string& str) -> std::string {
            return str;
        };
        std::ifstream tpl_fstream{ argv[1] };
        std::string tpl{ std::istreambuf_iterator<char>{ tpl_fstream },
                         std::istreambuf_iterator<char>{} };
        mstch::map ctx{
            {"include", mstch::lambda{ [](const std::string &filename) -> mstch::node {
                std::ifstream file_fstream{ filename };
                if((file_fstream.rdstate() & std::ifstream::failbit) != 0)
                    throw std::runtime_error("Can't open file: " + filename);
                return std::string { std::istreambuf_iterator<char>{ file_fstream },
                                     std::istreambuf_iterator<char>{} };
            }}},
            {"env", mstch::lambda{ [](const std::string &env_var_name) -> mstch::node {
                if(char *env_var_value = std::getenv(env_var_name.c_str()))
                    return std::string{env_var_value};
                throw std::runtime_error("Env var doesn't exist: " + env_var_name);
            }}},
            {"cmd", mstch::lambda{ [](const std::string &cmd) -> mstch::node {
                // https://stackoverflow.com/a/478960/547223
                std::array<char, 16384> buffer;  // 16 KiB
                buffer.fill('\0');
                std::string stdout;
                std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
                if(!pipe)
                    throw std::runtime_error("Command failed: " + cmd);
                while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
                    stdout += buffer.data();
                return stdout;
            }}}
        };

        for(int i=2; i+1<argc; i+=2)
            ctx.emplace(std::make_pair(std::string(argv[i]), std::string(argv[i+1])));

        std::cout << mstch::render(tpl, ctx);
    } catch (...) {
        std::cerr << boost::current_exception_diagnostic_information() << std::endl;
        return -1;
    }
    return 0;
}

