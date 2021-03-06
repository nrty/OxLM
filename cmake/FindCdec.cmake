# Ideally we should recursively check for cdec dependencies.

if(NOT DEFINED CDEC_DECODER_INCLUDE_DIR)
  find_path(CDEC_DECODER_INCLUDE_DIR
      NAMES decoder.h hg.h ff.h
      PATHS $ENV{CDEC}
      PATH_SUFFIXES decoder)
endif(NOT DEFINED CDEC_DECODER_INCLUDE_DIR)

if(NOT DEFINED CDEC_UTILS_INCLUDE_DIR)
  find_path(CDEC_UTILS_INCLUDE_DIR
      NAMES stringlib.h fdict.h
      PATHS $ENV{CDEC}
      PATH_SUFFIXES utils)
endif(NOT DEFINED CDEC_UTILS_INCLUDE_DIR)

if(CDEC_DECODER_INCLUDE_DIR AND CDEC_UTILS_INCLUDE_DIR)
  set(CDEC_INCLUDE_DIRS ${CDEC_DECODER_INCLUDE_DIR} ${CDEC_UTILS_INCLUDE_DIR})
  set(CDEC_FOUND TRUE)
else(CDEC_DECODER_INCLUDE_DIR AND CDEC_UTILS_INCLUDE_DIR)
  set(CDEC_FOUND FALSE)
endif(CDEC_DECODER_INCLUDE_DIR AND CDEC_UTILS_INCLUDE_DIR)

if(NOT DEFINED CDEC_DECODER_LIBRARIES)
  find_library(CDEC_DECODER_LIBRARIES
      NAMES libcdec.a
      PATHS $ENV{CDEC}
      PATH_SUFFIXES decoder)
endif(NOT DEFINED CDEC_DECODER_LIBRARIES)

if(NOT DEFINED CDEC_UTILS_LIBRARIES)
  find_library(CDEC_UTILS_LIBRARIES
      NAMES libutils.a
      PATHS $ENV{CDEC}
      PATH_SUFFIXES utils)
endif(NOT DEFINED CDEC_UTILS_LIBRARIES)

if(CDEC_DECODER_LIBRARIES AND CDEC_UTILS_LIBRARIES)
  set(CDEC_LIBRARIES ${CDEC_DECODER_LIBRARIES} ${CDEC_UTILS_LIBRARIES})
  set(CDEC_LIBRARIES_FOUND TRUE)
else(CDEC_DECODER_LIBRARIES AND CDEC_UTILS_LIBRARIES)
  set(CDEC_LIBRARIES_FOUND FALSE)
endif(CDEC_DECODER_LIBRARIES AND CDEC_UTILS_LIBRARIES)
