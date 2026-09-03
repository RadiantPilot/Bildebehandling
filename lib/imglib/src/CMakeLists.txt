add_library(imglib image.cpp file_bmp.cpp file_pgm.cpp file_pbm.cpp utils.cpp)
target_link_libraries(imglib PUBLIC m)
target_include_directories(imglib PUBLIC ../inc)
