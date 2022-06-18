=====
 mst
=====

.. vim: ro noma

A rather small CLI file templating tool written in Rust. Specifically designed to handle dotfiles
(see: `kgadek/yadm-bin <https://github.com/kgadek/yadm-bin>`_ - a yadm with ``mst`` templating).

   **Warning**: ``mst`` assumes input is trusted. With all flags, it **will run arbitrary commands**
   and **read arbitrary files** as instructed by the template file. These features can be enabled
   by compile-time flags: ``--features with_include,with_cmd``.

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

Build on macOS using::

    cargo build --release --all-features

When uploading to GitHub:

- remember to strip
- rename to ``mst-"$(uname -sm | tr ' ' -)``

To update ``README.rst`` run::

    mst README.mst.rst > README.rst

