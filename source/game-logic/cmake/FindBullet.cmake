# Locate Bullet.
#
# This script defines:
#   BULLET_FOUND, set to 1 if found
#   BULLET_LIBRARIES
#   BULLET_INCLUDE_DIR
#   BULLET_*_LIBRARY, one for each library (for example, "BULLET_BulletCollision_LIBRARY").
#   BULLET_*_LIBRARY_debug, one for each library.
#   BULLET_EXTRAS_INCLUDE_DIR - Directory containing the bullet "Extras" headers
#   BULLET_DEMOS_INCLUDE_DIR - Directory containing the Demos/OpenGL headers
#
# This script will look in standard locations for installed Bullet. However, if you
# install Bullet into a non-standard location, you can use the BULLET_ROOT
# variable (in environment or CMake) to specify the location.
#
# You can also use Bullet out of a source tree by specifying BULLET_SOURCE_DIR
# and BULLET_BUILD_DIR (in environment or CMake).

message(STATUS "Looking for Bullet...")

mark_as_advanced(BULLET_INCLUDE_DIR)

set(BULLET_INCLUDE_DIR ${LIB_DIR}/bullet-2.80-rev2531/src)
set(BULLET_INCLUDE_LINEARMATH_DIR ${LIB_DIR}/bullet-2.80-rev2531/src/LinearMath)
set(BULLET_INCLUDE_SOFTBODY_DIR ${LIB_DIR}/bullet-2.80-rev2531/src/BulletSoftBody)
set(BULLET_INCLUDE_COLLISION_DIR ${LIB_DIR}/bullet-2.80-rev2531/src/BulletCollision)
set(BULLET_INCLUDE_DYNAMICS_DIR ${LIB_DIR}/bullet-2.80-rev2531/src/BulletDynamics)
set(BULLET_INCLUDE_WORLDIMPORTER_DIR ${LIB_DIR}/bullet-2.80-rev2531/Extras/Serialize/BulletWorldImporter)
set(BULLET_INCLUDE_FILELOADER_DIR ${LIB_DIR}/bullet-2.80-rev2531/Extras/Serialize/BulletFileLoader)
set(BULLET_INCLUDE_CONVEXDECOMPOSITION_DIR ${LIB_DIR}/bullet-2.80-rev2531/Extras/ConvexDecomposition)

find_path(BULLET_INCLUDE_DIR btBulletCollisionCommon.h
          PATHS ${HINT_PATHS})

find_path(BULLET_INCLUDE_CONVEXDECOMPOSITION_DIR ConvexDecomposition/ConvexDecomposition.h
          PATHS ${HINT_PATHS})

if(BULLET_INCLUDE_CONVEXDECOMPOSITION_DIR)
        set(BULLET_INCLUDE_CONVEXDECOMPOSITION_DIR ${LIB_DIR}/bullet-2.80-rev2531/Extras/ConvexDecomposition)
endif(BULLET_INCLUDE_CONVEXDECOMPOSITION_DIR)

find_path(BULLET_INCLUDE_SOFTBODY_DIR BulletSoftBody/btSoftBody.h
          PATHS ${HINT_PATHS})

if(BULLET_INCLUDE_SOFTBODY_DIR)
        set(BULLET_INCLUDE_SOFTBODY_DIR ${LIB_DIR}/bullet-2.80-rev2531/src/BulletSoftBody)
endif(BULLET_INCLUDE_SOFTBODY_DIR)

find_path(BULLET_INCLUDE_LINEARMATH_DIR LinearMath/btScalar.h
          PATHS ${HINT_PATHS})

if(BULLET_INCLUDE_LINEARMATH_DIR)
        set(BULLET_INCLUDE_LINEARMATH_DIR ${LIB_DIR}/bullet-2.80-rev2531/src/LinearMath)
endif(BULLET_INCLUDE_LINEARMATH_DIR)

mark_as_advanced(BULLET_INCLUDE_COLLISION_DIR
                 BULLET_INCLUDE_DYNAMICS_DIR
                 BULLET_INCLUDE_CONVEXDECOMPOSITION_DIR
				 BULLET_INCLUDE_SOFTBODY_DIR
                 BULLET_INCLUDE_LINEARMATH_DIR
				 BULLET_INCLUDE_FILELOADER_DIR
				 BULLET_INCLUDE_WORLDIMPORTER_DIR
				 )

#else(OSX)
#        find_path(BULLET_INCLUDE_COLLISION_DIR BulletCollision/btBulletCollisionCommon.h
#                  PATHS ${HINT_PATHS})

#        if(BULLET_INCLUDE_COLLISION_DIR)
#                 set(BULLET_INCLUDE_DIR "${BULLET_INCLUDE_COLLISION_DIR}/../")
#                 mark_as_advanced(BULLET_INCLUDE_COLLISION_DIR)
#        endif()

#        find_path(BULLET_INCLUDE_DYNAMICS_DIR BulletDynamics/btBulletDynamicsCommon.h
#                  PATHS ${HINT_PATHS})
#        mark_as_advanced(BULLET_INCLUDE_DYNAMICS_DIR)

#        find_path(BULLET_INCLUDE_CONVEXDECOMPOSITION_DIR ConvexDecomposition/ConvexDecomposition.h)
#        mark_as_advanced(BULLET_INCLUDE_CONVEXDECOMPOSITION_DIR)

#        find_path(BULLET_INCLUDE_SOFTBODY_DIR BulletSoftBody/btSoftBody.h)
#        mark_as_advanced(BULLET_INCLUDE_SOFTBODY_DIR)
#endif(OSX)

set(BULLET_INCLUDE_DIRS
        ${BULLET_INCLUDE_DIR}
        ${BULLET_INCLUDE_COLLISION_DIR}
        ${BULLET_INCLUDE_DYNAMICS_DIR}
        ${BULLET_INCLUDE_CONVEXDECOMPOSITION_DIR}
        ${BULLET_INCLUDE_SOFTBODY_DIR}
        ${BULLET_INCLUDE_LINEARMATH_DIR}
		${BULLET_INCLUDE_FILELOADER_DIR}
		${BULLET_INCLUDE_WORLDIMPORTER_DIR}
		)

message(STATUS "BULLET_INCLUDE_DIRS: ${BULLET_INCLUDE_DIRS}")

if(UNIX)
set(BULLET_COLLISION_LIBRARY ${LIB_DIR}/bullet-2.80-rev2531/libs/linux64/libBulletCollision.a)
set(BULLET_DYNAMICS_LIBRARY ${LIB_DIR}/bullet-2.80-rev2531/libs/linux64/libBulletDynamics.a)
set(BULLET_FILELOADER_LIBRARY ${LIB_DIR}/bullet-2.80-rev2531/libs/linux64/libBulletFileLoader.a)
set(BULLET_SOFTBODY_LIBRARY ${LIB_DIR}/bullet-2.80-rev2531/libs/linux64/libBulletSoftBody.a)
set(BULLET_WORLDIMPORTER_LIBRARY ${LIB_DIR}/bullet-2.80-rev2531/libs/linux64/libBulletWorldImporter.a)
set(BULLET_CONVEXDECOMPOSITION_LIBRARY ${LIB_DIR}/bullet-2.80-rev2531/libs/linux64/libConvexDecomposition.a)
set(BULLET_LINEARMATH_LIBRARY ${LIB_DIR}/bullet-2.80-rev2531/libs/linux64/libLinearMath.a)
endif(UNIX)

find_library(BULLET_COLLISION_LIBRARY BulletCollision)
mark_as_advanced(BULLET_COLLISION_LIBRARY)

find_library(BULLET_DYNAMICS_LIBRARY BulletDynamics)
mark_as_advanced(BULLET_DYNAMICS_LIBRARY)

find_library(BULLET_CONVEXDECOMPOSITION_LIBRARY ConvexDecomposition)
mark_as_advanced(BULLET_CONVEXDECOMPOSITION_LIBRARY)

find_library(BULLET_SOFTBODY_LIBRARY BulletSoftBody)
mark_as_advanced(BULLET_SOFTBODY_LIBRARY)

find_library(BULLET_LINEARMATH_LIBRARY LinearMath)
mark_as_advanced(BULLET_LINEARMATH_LIBRARY)

find_library(BULLET_FILELOADER_LIBRARY FileLoader)
mark_as_advanced(BULLET_FILELOADER_LIBRARY)

find_library(BULLET_WORLDIMPORTER_LIBRARY WorldImporter)
mark_as_advanced(BULLET_WORLDIMPORTER_LIBRARY)

if(BULLET_INCLUDE_DIR AND NOT OSX)
        set(BULLET_EXTRAS_INCLUDE_DIRS ${BULLET_INCLUDE_DIR}/../Extras)
        set(BULLET_DEMOS_INCLUDE_DIRS ${BULLET_INCLUDE_DIR}/../Demos/OpenGL)
endif()

macro(FIND_BULLET_LIBRARY_DIRNAME LIBNAME DIRNAME)
message(STATUS "Looking for ${LIBNAME}...")
    unset(BULLET_${LIBNAME}_LIBRARY CACHE)
    unset(BULLET_${LIBNAME}_LIBRARY_debug CACHE)
    mark_as_advanced(BULLET_${LIBNAME}_LIBRARY)
    mark_as_advanced(BULLET_${LIBNAME}_LIBRARY_debug)
    find_library(BULLET_${LIBNAME}_LIBRARY
        NAMES ${LIBNAME}
        PATHS
            ${BULLET_ROOT}
            $ENV{BULLET_ROOT}
            $ENV{BULLET_LIBS}
            ${BULLET_BUILD_DIR}
            $ENV{BULLET_BUILD_DIR}
            "C:/Program Files/BULLET_PHYSICS"
			${LIB_DIR}/bullet-2.80-rev2531/libs
			${LIB_DIR}/bullet-2.80-rev2531/src/libs
        PATH_SUFFIXES x64
            ./src/${DIRNAME}
            ./Extras/${DIRNAME}
            ./Demos/${DIRNAME}
            ./src/${DIRNAME}/Release
            ./Extras/${DIRNAME}/Release
            ./Demos/${DIRNAME}/Release
            ./libs/${DIRNAME}
            ./libs
            ./lib
            ./libs/Linuxt64
            ./lib/Release) # v2.76, new location for build tree libs on Windows

    find_library(BULLET_${LIBNAME}_LIBRARY_debug
        NAMES ${LIBNAME}_Debug
        PATHS
            ${BULLET_ROOT}
            $ENV{BULLET_ROOT}
            ${BULLET_BUILD_DIR}
            $ENV{BULLET_BUILD_DIR}
            "C:/Program Files/BULLET_PHYSICS"
			${LIB_DIR}/bullet-2.80-rev2531/libs
        PATH_SUFFIXES x64
            ./src/${DIRNAME}
            ./Extras/${DIRNAME}
            ./Demos/${DIRNAME}
            ./src/${DIRNAME}/Debug
            ./Extras/${DIRNAME}/Debug
            ./Demos/${DIRNAME}/Debug
            ./libs/${DIRNAME}
            ./libs
            ./lib
            ${LIB_DIR}/bullet-2.80-rev2531/libs
            ./lib/Debug) # v2.76, new location for build tree libs on Windows

#    message(STATUS ${BULLET_${LIBNAME}_LIBRARY} ${BULLET_${LIBNAME}_LIBRARY_debug})
#    message(SEND_ERROR ${LIBNAME})

    if(BULLET_${LIBNAME}_LIBRARY)
        set(BULLET_LIBRARIES ${BULLET_LIBRARIES} "optimized" ${BULLET_${LIBNAME}_LIBRARY})
        message(STATUS "Found Bullet${LIBNAME}: ${BULLET_${LIBNAME}_LIBRARY}")
    else(BULLET_${LIBNAME}_LIBRARY)
        message(STATUS "Bullet${LIBNAME} not found!")
    endif(BULLET_${LIBNAME}_LIBRARY)

    if(BULLET_${LIBNAME}_LIBRARY_debug)
        set(BULLET_LIBRARIES ${BULLET_LIBRARIES}
            "debug" ${BULLET_${LIBNAME}_LIBRARY_debug})
        message(STATUS "Found Bullet${LIBNAME} (debug): ${BULLET_${LIBNAME}_LIBRARY_debug}")
    else(BULLET_${LIBNAME}_LIBRARY_debug)
        message(STATUS "Bullet${LIBNAME} (debug) not found!")
    endif(BULLET_${LIBNAME}_LIBRARY_debug)

endmacro(FIND_BULLET_LIBRARY_DIRNAME LIBNAME)

macro(FIND_BULLET_LIBRARY LIBNAME)
        find_bullet_library_dirname(${LIBNAME} ${LIBNAME})
endmacro(FIND_BULLET_LIBRARY LIBNAME)

if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(BULLET_LIBRARIES
            ${BULLET_COLLISION_LIBRARY}
            ${BULLET_DYNAMICS_LIBRARY}
            ${BULLET_CONVEXDECOMPOSITION_LIBRARY}
            ${BULLET_SOFTBODY_LIBRARY}
            ${BULLET_LINEARMATH_LIBRARY}
			${BULLET_FILELOADER_LIBRARY}
			${BULLET_WORLDIMPORTER_LIBRARY}
			)
else()
    find_bullet_library(BulletDynamics)
    find_bullet_library(BulletSoftBody)
    find_bullet_library(BulletCollision)
    #find_bullet_library(BulletMultiThreaded)
    find_bullet_library(LinearMath)
    #find_bullet_library_dirname(OpenGLSupport OpenGL)
    find_bullet_library(ConvexDecomposition)
	find_bullet_library(BulletFileLoader)
	find_bullet_library(BulletWorldImporter)
endif()

# Hide BULLET_LIBRARY in the GUI, since most users can just ignore it
#mark_as_advanced(BULLET_LIBRARIES)
#mark_as_advanced(BULLET_LIBRARIES_debug)

set(BULLET_FOUND FALSE)
if(BULLET_INCLUDE_DIR AND BULLET_LIBRARIES)
        set(BULLET_FOUND TRUE)
endif(BULLET_INCLUDE_DIR AND BULLET_LIBRARIES)

# in v2.76, ColladaConverter was removed.
#if(BULLET_BulletColladaConverter_LIBRARY OR BULLET_BulletColladaConverter_LIBRARY_debug)
#   set(BULLET_COLLADACONVERTER_FOUND)
#endif(BULLET_BulletColladaConverter_LIBRARY OR BULLET_BulletColladaConverter_LIBRARY_debug)
