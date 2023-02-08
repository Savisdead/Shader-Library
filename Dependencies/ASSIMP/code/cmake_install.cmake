# Install script for directory: /c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump.exe")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibassimp5.2.5-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/lib/libassimp.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibassimp5.2.5x" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/bin/msys-assimp-5.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/msys-assimp-5.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/msys-assimp-5.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/msys-assimp-5.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/anim.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/aabb.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/ai_assert.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/camera.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/color4.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/color4.inl"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/config.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/ColladaMetaData.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/commonMetaData.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/defs.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/cfileio.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/light.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/material.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/material.inl"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/matrix3x3.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/matrix3x3.inl"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/matrix4x4.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/matrix4x4.inl"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/mesh.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/ObjMaterial.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/pbrmaterial.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/GltfMaterial.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/postprocess.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/quaternion.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/quaternion.inl"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/scene.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/metadata.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/texture.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/types.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/vector2.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/vector2.inl"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/vector3.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/vector3.inl"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/version.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/cimport.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/importerdesc.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/Importer.hpp"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/DefaultLogger.hpp"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/ProgressHandler.hpp"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/IOStream.hpp"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/IOSystem.hpp"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/Logger.hpp"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/LogStream.hpp"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/NullLogger.hpp"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/cexport.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/Exporter.hpp"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/DefaultIOStream.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/DefaultIOSystem.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/ZipArchiveIOSystem.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/SceneCombiner.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/fast_atof.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/qnan.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/BaseImporter.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/Hash.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/MemoryIOWrapper.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/ParsingUtils.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/StreamReader.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/StreamWriter.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/StringComparison.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/StringUtils.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/SGSpatialSort.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/GenericProperty.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/SpatialSort.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/SkeletonMeshBuilder.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/SmallVector.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/SmoothingGroups.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/SmoothingGroups.inl"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/StandardShapes.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/RemoveComments.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/Subdivision.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/Vertex.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/LineSplitter.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/TinyFormatter.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/Profiler.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/LogAux.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/Bitmap.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/XMLTools.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/IOStreamBuffer.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/CreateAnimMesh.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/XmlParser.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/BlobIOSystem.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/MathFunctions.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/Exceptional.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/ByteSwapper.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/Base64.hpp"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/Compiler/pushpack1.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/Compiler/poppack1.h"
    "/c/OpenGLS/Project/Project1/Dependencies/ASSIMP/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

