# plotjuggler kvaser data loader

This repository contains a
[PlotJuggler](https://github.com/facontidavide/PlotJuggler) plugin that can
read csv files exported by [kvaser memorator
devices](https://kvaser.com/product/kvaser-memorator-professional-2xhs-v2/).

To decode CAN frames, the plugin uses [fcp-cpp](https://github.com/joajfreitas/fcp-cpp).

## Build

```
mkdir build
cd build
cmake ..
make -j$(nproc)
sudo make install
```

Look at the [CMakeLists.txt](CMakeLists.txt) file to learn how to
find **Qt** and PlotJuggler.

## Plugin installation

```
cd build
sudo make install
