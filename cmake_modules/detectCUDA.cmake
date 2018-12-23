include(CheckLanguage)
check_language(CUDA)
if(CMAKE_CUDA_COMPILER)
  enable_language(CUDA)
  #Autodetect CUDA architecture
  include(${CMAKE_SOURCE_DIR}/cmake_modules/detectCUDAarch.cmake)
  set(cuda_lang "$<COMPILE_LANGUAGE:CUDA>")
  set(debug_cuda_lang "$<AND:$<CONFIG:DEBUG>,${cuda_lang}>")
  add_library(cuda_compile_info INTERFACE)
  target_compile_options(cuda_compile_info INTERFACE
    $<${cuda_lang}:$<BUILD_INTERFACE:-Xcompiler=-Wall>>
    $<${debug_cuda_lang}:$<BUILD_INTERFACE:-Xcompiler=-Wunused-parameter>>
  )
  target_compile_options(cuda_compile_info INTERFACE
      $<${cuda_lang}:$<BUILD_INTERFACE:${CS40_CUDA_ARCH}>>
  )
else()
  message(STATUS
  "Could not find CUDA, set CMAKE_CUDA_COMPILER to\n
   full path to nvcc if you think this is an error")
  #Optionally, add path to nvcc to PATH env variable
endif()
