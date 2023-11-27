rm -rf build && mkdir build && cd build && cmake .. && make

./buildHistogram ../resources/pic.1016.jpg ../resources baseline 3
wait