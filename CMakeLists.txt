cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
									# Если версия установленой программы
									# старее указаной, произайдёт аварийный выход.

project(bmp_files)


set(SOURCE_EXE main.cpp)		# Установка переменной со списком исходников для исполняемого файла

set(SOURCE_LIB bmp.cpp bmp_filters.cpp bmp.h)			# То же самое, но для библиотеки

add_library(bmp STATIC ${SOURCE_LIB})	# Создание статической библиотеки с именем bmp

add_executable(main ${SOURCE_EXE})	# Создает исполняемый файл с именем main

target_link_libraries(main bmp)		# Линковка программы с библиотекой 