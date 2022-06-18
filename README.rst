=====
 mst
=====

.. vim: ro noma

A rather small CLI file templating tool written in C++. Specifically designed to handle dotfiles
(see: `kgadek/yadm-bin <https://github.com/kgadek/yadm-bin>`_ - a yadm with ``mst`` templating).

Standing on a shoulders of a wonderful `no1msd/mstch <https://github.com/no1msd/mstch>`_ library.

   **Warning ⚠️**: ``mst`` assumes input is trusted. By design, it **will run arbitrary commands**
   and **read arbitrary files** as instructed by input file. These features can be disabled
   by compile-time flags: ``cmake -DWITH_CMD=OFF -DWITH_INCLUDE=OFF ...``.

.. highlight:: bash
.. code-block:: console

   konrad@pc mst$ tokei .
   ===============================================================================
    Language            Files        Lines         Code     Comments       Blanks
   ===============================================================================
    Rust                    1           76           67            0            9
    TOML                    1           11            9            0            2
   ===============================================================================
    Total                   2           87           76            0           11
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

To update ``README.rst`` run::

    mst README.mst.rst > README.rst

