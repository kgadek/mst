# Notes

The `no1msd/mstch` was added to this repo via:
```bash
git subtree add --prefix mstch git@github.com:no1msd/mstch.git master --squash
```

Build on macOS using:
```bash
brew install cmake boost  # dependencies

mkdir build/
cd build/
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

On Linux, you can build static binary by using different cmake invocation:
```bash
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DCMAKE_EXE_LINKER_FLAGS="-static" -DCMAKE_FIND_LIBRARY_SUFFIXES=".a" ..
```

When uploading to GitHub:
- remember to strip
- rename to `mst-"$(uname -sm | tr ' ' -)`
