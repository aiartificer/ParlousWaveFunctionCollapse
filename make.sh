g++ -Iinclude -I../lua-5.1.5/src -I ParlousWaveFunctionCollapse/include -L../lua-5.1.5/src -c ParlousWaveFunctionCollapse/include/parlous_array.h -O -Wall -o ParlousWaveFunctionCollapse/lib/linux/parlous_array.o -llua -fPIC
g++ -shared -o ParlousWaveFunctionCollapse/lib/linux/libParlousArray.so ParlousWaveFunctionCollapse/lib/linux/parlous_array.o
g++ -Iinclude -I../lua-5.1.5/src -I ParlousWaveFunctionCollapse/include -L../lua-5.1.5/src -c ParlousWaveFunctionCollapse/src/hexmap_helper.cpp -O -Wall -o ParlousWaveFunctionCollapse/lib/linux/hexmap_helper.o -llua -fPIC
g++ -shared -o ParlousWaveFunctionCollapse/lib/linux/libHexMapHelper.so ParlousWaveFunctionCollapse/lib/linux/hexmap_helper.o
g++ -Iinclude -I../lua-5.1.5/src -I ParlousWaveFunctionCollapse/include -L../lua-5.1.5/src -L ParlousWaveFunctionCollapse/lib/linux -l HExMapHelper test/test.cpp ParlousWaveFunctionCollapse/src/hexmap_helper.cpp ParlousWaveFunctionCollapse/src/parlous_wfc.cpp -O -Wall -o parlous_wfc -llua
