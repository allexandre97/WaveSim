ADD_LIBRARY(Algebra algebra.cpp)
ADD_LIBRARY(Builder builder.cpp)
ADD_LIBRARY(Reader  reader.cpp)
ADD_LIBRARY(Saver   saver.cpp)
ADD_LIBRARY(Solver  solver.cpp)

target_include_directories(Algebra INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE   ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(Builder INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE   ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(Reader  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE   ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(Saver   INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE   ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(Solver  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE   ${CMAKE_CURRENT_BINARY_DIR})


FIND_PACKAGE(fmt REQUIRED)
INCLUDE_DIRECTORIES(${fmt_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES(Algebra PUBLIC compiler_flags)
TARGET_LINK_LIBRARIES(Builder PUBLIC compiler_flags fmt::fmt)
TARGET_LINK_LIBRARIES(Reader  PUBLIC compiler_flags)
TARGET_LINK_LIBRARIES(Saver   PUBLIC compiler_flags)
TARGET_LINK_LIBRARIES(Solver  PUBLIC compiler_flags fmt::fmt Algebra Saver)


SET(INSTALLABLE_LIBS Algebra Builder Reader Saver Solver compiler_flags)
SET(INSTALLABLE_HEADS algebra.hpp builder.hpp reader.hpp saver.hpp solver.hpp)
INSTALL(TARGETS ${INSTALLABLE_LIBS} DESTINATION lib)
INSTALL(FILES ${INSTALLABLE_HEADS} DESTINATION include)
