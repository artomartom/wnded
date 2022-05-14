cmake_minimum_required(VERSION 3.22)     
project(Hello_test)
set(CMAKE_CXX_STANDARD 20)
add_executable(${PROJECT_NAME}    ${PROJECT_SOURCE_DIR}/window.cpp)


option(MAKEGUI "build  as a GUI rather then console app" OFF)   

  if(MAKEGUI)
    set_target_properties(${PROJECT_NAME} PROPERTIES   WIN32_EXECUTABLE TRUE)
   target_compile_definitions(${PROJECT_NAME}  PUBLIC  GUI )
 else()
    target_compile_definitions(${PROJECT_NAME}  PUBLIC   CONSOLE  )
     
      
  endif(MAKEGUI)
unset(MAKEGUI CACHE) # <---- this is important 


 if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
target_compile_options(${PROJECT_NAME} PRIVATE  
/Wall #enable all warnings
/options:strict # unrecognized compiler options are an error
/nologo #suppress copyright message    
/analyze #Enable native analysis      
/external:anglebrackets #treat all headers included via <> as external
/external:W0
)
endif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")


target_precompile_headers(${PROJECT_NAME} PRIVATE pch.hpp)

