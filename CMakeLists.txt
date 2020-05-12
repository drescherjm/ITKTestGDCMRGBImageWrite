cmake_minimum_required(VERSION 3.10.2)

project( ITKTestGDCMRGBImageWrite )

find_package( ITK REQUIRED )
include( ${ITK_USE_FILE} )

add_executable( ITKTestGDCMRGBImageWrite Code.cxx )
target_link_libraries( ITKTestGDCMRGBImageWrite ${ITK_LIBRARIES} )