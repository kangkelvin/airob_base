cmake_minimum_required(VERSION 3.16)
project(airob_base)

###### add and load monorepo script ########
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/infra/CMakeModules)
include(mono_repo)

load_all_components()
