#
# Copyright 2011-2013 Free Software Foundation, Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

########################################################################
# Setup profiler
########################################################################
if(MSVC)
    include_directories(${CMAKE_SOURCE_DIR}/cmake/msvc)
endif(MSVC)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_BINARY_DIR}/lib
    ${Boost_INCLUDE_DIRS}
)

# MAKE volk_boxmuller_profile
add_executable(volk_boxmuller_profile
    ${CMAKE_CURRENT_SOURCE_DIR}/volk_boxmuller_profile.cc
    ${CMAKE_SOURCE_DIR}/lib/qa_utils.cc
)


if(ENABLE_STATIC_LIBS)
    target_link_libraries(volk_boxmuller_profile volk_boxmuller_static ${Boost_LIBRARIES})
    set_target_properties(volk_boxmuller_profile PROPERTIES LINK_FLAGS "-static")
else()
    target_link_libraries(volk_boxmuller_profile volk_boxmuller ${Boost_LIBRARIES})
endif()

install(
    TARGETS volk_boxmuller_profile
    DESTINATION bin
    COMPONENT "volk_boxmuller"
)

# MAKE volk_boxmuller-config-info
add_executable(volk_boxmuller-config-info volk_boxmuller-config-info.cc)

if(ENABLE_STATIC_LIBS)
    target_link_libraries(volk_boxmuller-config-info volk_boxmuller_static ${Boost_LIBRARIES})
    set_target_properties(volk_boxmuller-config-info PROPERTIES LINK_FLAGS "-static")
else()
    target_link_libraries(volk_boxmuller-config-info volk_boxmuller ${Boost_LIBRARIES})
endif()

install(
    TARGETS volk_boxmuller-config-info
    DESTINATION bin
    COMPONENT "volk_boxmuller"
)
