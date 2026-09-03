add_executable(main main.cpp)
		    
target_link_libraries(main PUBLIC imglib)
target_link_libraries(main PUBLIC m)
target_include_directories(main PUBLIC "${PROJECT_SOURCE_DIR}/inc")
target_include_directories(main PUBLIC "${PROJECT_SOURCE_DIR}/lib")
