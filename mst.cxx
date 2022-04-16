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
#ifdef MST_WITH_CMD
#include "pstream.h"
#endif

int main(int argc, char* argv[]) {
  using namespace std;
  try {
    mstch::config::escape = [](const string& str) -> string { return str; };
    ifstream template_file{argv[1]};
    string template_content{istreambuf_iterator<char>{template_file}, istreambuf_iterator<char>{}};
    mstch::map ctx{{"include", mstch::lambda{[](const string& filename) -> mstch::node {
                      ifstream file_fstream{filename};
                      if ((file_fstream.rdstate() & ifstream::failbit) != 0) {
                        throw runtime_error("Can't open file: " + filename);
                      }
                      return string{istreambuf_iterator<char>{file_fstream}, istreambuf_iterator<char>{}};
                    }}},
#ifdef MST_WITH_CMD
                   {"cmd", mstch::lambda{[](const string& cmd) -> mstch::node {
                      redi::ipstream cmd_stream(cmd);
                      string stdout{istreambuf_iterator<char>{cmd_stream}, istreambuf_iterator<char>{}};
                      cmd_stream.close();
                      if (int exitcode = cmd_stream.rdbuf()->status(); cmd_stream.rdbuf()->exited() && exitcode != 0) {
                        throw runtime_error("Exit status " + to_string(exitcode) + " of command: '" + cmd + "'");
                      }
                      return stdout;
                    }}},
#endif
                   {"env", mstch::lambda{[](const string& env_var_name) -> mstch::node {
                      if (char* env_var_value = getenv(env_var_name.c_str())) {
                        return string{env_var_value};
                      }
                      throw runtime_error("Env var doesn't exist: " + env_var_name);
                    }}}};

    for (int i = 2; i + 1 < argc; i += 2) {
      ctx.emplace(make_pair(string(argv[i]), string(argv[i + 1])));
    }
    cout << mstch::render(template_content, ctx);
    return 0;
  } catch (...) {
    cerr << boost::current_exception_diagnostic_information() << endl << "Invocation:\n";
    for (int i = 0; i < argc; ++i) {
      cerr << "  '" << argv[i] << "'" << endl;
    }
    return -1;
  }
}
