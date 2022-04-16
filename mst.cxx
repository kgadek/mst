// vim: sw=4 ts=4 expandtab cc=120

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0
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

int main(int argc, char* argv[]) {
  using namespace std;
  try {
    mstch::config::escape = [](const string& str) -> string { return str; };
    ifstream tpl_fstream{argv[1]};
    string tpl{istreambuf_iterator<char>{tpl_fstream}, istreambuf_iterator<char>{}};
    mstch::map ctx{{"include", mstch::lambda{[](const string& filename) -> mstch::node {
                      ifstream file_fstream{filename};
                      if ((file_fstream.rdstate() & ifstream::failbit) != 0)
                        throw runtime_error("Can't open file: " + filename);
                      return string{istreambuf_iterator<char>{file_fstream}, istreambuf_iterator<char>{}};
                    }}},
#ifdef MST_WITH_CMD
                   {"cmd", mstch::lambda{[](const string& cmd) -> mstch::node {
                      // https://stackoverflow.com/a/478960/547223
                      array<char, 16384> buffer;  // 16 KiB
                      buffer.fill('\0');
                      string stdout;
                      unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
                      if (!pipe) throw runtime_error("Command failed: " + cmd);
                      while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) stdout += buffer.data();
                      return stdout;
                    }}},
#endif
                   {"env", mstch::lambda{[](const string& env_var_name) -> mstch::node {
                      if (char* env_var_value = getenv(env_var_name.c_str())) return string{env_var_value};
                      throw runtime_error("Env var doesn't exist: " + env_var_name);
                    }}}};

    for (int i = 2; i + 1 < argc; i += 2) ctx.emplace(make_pair(string(argv[i]), string(argv[i + 1])));

    cout << mstch::render(tpl, ctx);
  } catch (...) {
    cerr << "Invocation:\n" << endl;
    for (int i = 0; i < argc; ++i) cerr << "  '" << argv[i] << "'" << endl;
    cerr << boost::current_exception_diagnostic_information() << endl;
    return -1;
  }
  return 0;
}
