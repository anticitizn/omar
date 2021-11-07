#### OMAR - **O**pengl bit**M**ap **A**scii **R**enderer
---
![omar-gif](https://user-images.githubusercontent.com/33865403/107491814-f16cd600-6b8b-11eb-9ba7-f8db69026ec5.gif)

Omar is a barebones library for rendering and handling bitmap text on the screen. It abstracts away most graphical details and lets you play with the text field nicely without flickering or whatever issues that sometimes arise when drawing to normal terminal emulators. It's also hardware-accelerated and runs anywhere where SDL does, so it should be pretty portable. Some limited support for making GUIs inside of it is coming soon™.

#### How is this better than libtcod or similar alternatives?
It's not, except that this one is mine :D

#### Usage
If for whatever reason you want to try it out, you're pretty much forced to use CMake. Put it anywhere in your project and then add `add_subdirectory("folder_where_you_put_it/omar")` to your CMakeLists file. Include it by `#include <src/omar.h>`. A quick example for usage is included in the `main.cpp` file in this repository.
