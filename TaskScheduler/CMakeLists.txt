﻿# CMakeList.txt: проект CMake для CMakeProject1; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Добавьте источник в исполняемый файл этого проекта.
add_executable (CMakeProject1 "TaskSchedulerTest.cpp" "TaskSchedulerTest.h" "Thread.cpp" "Thread.h" "TaskProcessor.cpp" "TaskProcessor.h"
				"Request.h" "TaskHolder.cpp" "TaskHolder.h")

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
