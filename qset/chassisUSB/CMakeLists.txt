cmake_minimum_required(VERSION "3.28.3")

project("USB")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find LIBUSB

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)

include_directories(${LIBUSB_INCLUDE_DIRS})
link_directories(${LIBUSB_LIBRARY_DIRS})

add_executable(main 
    chassis_board.h 
    chassis_board.cpp 
    dependencies/usb_chassis_defs.h 
    dependencies/usb_dev.h 
    dependencies/usb_packet.h
)
target_link_libraries(main ${LIBUSB_LIBRARIES})


