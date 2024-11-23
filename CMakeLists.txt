#================================================================================
cmake_minimum_required(VERSION 3.20)
set(CMAKE_TOOLCHAIN_FILE cmake/arm-none-eabi-gcc.cmake)



#================================================================================
# project(mk_23_kr-master LANGUAGES C CXX ASM)
 project(KURS_SAHAROVA1  LANGUAGES CXX C ASM)
 set(DEVICE stm32f407vg)
# set(DEVICE stm32f303vc)
# set(DEVICE stm32f103c8)

include(libopencm3)
include_directories(${PROJECT_NAME}/ring_buf)




#================================================================================
add_executable(${PROJECT_NAME} 
   src/main.cpp 
   ring_buffer/Ring_buf.cpp
   )
target_link_libraries(${PROJECT_NAME}
     PRIVATE libopencm3 )

set_target_properties(${PROJECT_NAME}
          PROPERTIES 

          CXX_STANDARD 17
          CXX_STANDARD_REQUIRED YES 
          CXX_EXTENSIONS NO
 

)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
     COMMAND cp ${PROJECT_NAME} firmware
)

#================================================================================

set(PROG stlink)
string(CONCAT PROG_TARGET ${LOCM3_DEVICE_SERIE} x)

include(oocd_flash_script)

install(CODE ${OOCD_FLASH_SCRIPT})
