=====
 mst
=====

A rather small CLI file templating tool written in C++. Specifically designed to handle dotfiles
(see: `kgadek/yadm-bin <https://github.com/kgadek/yadm-bin>`_ - a yadm with ``mst`` templating).

Standing on a shoulders of a wonderful `no1msd/mstch <https://github.com/no1msd/mstch>`_ library.

   **Warning ⚠️**: ``mst`` assumes input is trusted. By design, it **will run arbitrary commands**
   specified in templates with ``{{#cmd}}ls -alh *{{/cmd}}`` syntax. This feature can be disabled
   by a compile-time flag: ``cmake -DWITH_CMD=OFF ...``.

.. highlight:: bash
.. code-block:: console

   konrad@pc mst$ tokei .
   ===============================================================================
    Language            Files        Lines         Code     Comments       Blanks
   ===============================================================================
    CMake                   1           25           16            3            6
    C++                     1           65           56            6            3
    ReStructuredText        1           51           36            0           15
   ===============================================================================
    Total                   3          141          108            9           24
   ===============================================================================

Notes
=====

The ``no1msd/mstch`` was added to this repo via::

    git subtree add --prefix mstch git@github.com:no1msd/mstch.git master --squash

Build on macOS using::

    brew install cmake boost  # dependencies

    mkdir build/
    cd build/
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make

On Linux, you can build static binary by using different cmake invocation::

    cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DCMAKE_EXE_LINKER_FLAGS="-static" -DCMAKE_FIND_LIBRARY_SUFFIXES=".a" ..

When uploading to GitHub:

- remember to strip
- rename to ``mst-"$(uname -sm | tr ' ' -)``
