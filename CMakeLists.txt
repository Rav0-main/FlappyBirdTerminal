cmake_minimum_required(VERSION 3.10)
project(FlappyBirdTerminal VERSION 1.0.0 LANGUAGES CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Опции сборки
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Поиск всех .cpp файлов в директории src (если существует)
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Если нет src директории, ищем в текущей
if(NOT SOURCES)
    file(GLOB_RECURSE SOURCES "*.cpp")
endif()

# Создание исполняемого файла
add_executable(${PROJECT_NAME} ${SOURCES})

# Настройка путей для заголовочных файлов
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Включение предупреждений
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Настройка типа сборки по умолчанию
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()
