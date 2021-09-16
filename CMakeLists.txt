cmake_minimum_required(VERSION 3.1.0)
set(CXX_STANDARD 17)
set(CXX_VISIBILITY_PRESET true)
set(LIBNAME dcmanon)
set(EXENAME test)
project(DCMAnon)

add_library(${LIBNAME} SHARED dcmanon.cpp dcostrms.cc)
add_executable(${EXENAME} executer.cpp)

# using defualt lib

find_package(DCMTK CONFIG REQUIRED)
target_link_libraries(${LIBNAME} PUBLIC cmr i2d ijg8 dcmfg)

find_package(cppnetlib CONFIG REQUIRED)
target_include_directories(${LIBNAME} PRIVATE ${CPPNETLIB_INCLUDE_DIRS})
target_link_libraries(${LIBNAME} PRIVATE cppnetlib-uri cppnetlib-server-parsers cppnetlib-client-connections)   



# using static lib explicit
set(VCPKG_TARGET_TRIPLET x64-windows-static)
message(${VCPKG_TARGET_TRIPLET})
find_package(CURL REQUIRED)
target_link_libraries(${LIBNAME} PRIVATE ${CURL_LIBRARIES})
