add_subdirectory(${3RD_PARTY_DIR}/eigen)
add_subdirectory(${3RD_PARTY_DIR}/spdlog)
add_subdirectory(${3RD_PARTY_DIR}/googletest)
add_subdirectory(${3RD_PARTY_DIR}/whereami)

#include the 3rdparty headers
set(3RD_PARTY_INCLUDE_DIRS
    ${3RD_PARTY_DIR}/eigen
    ${3RD_PARTY_DIR}/spdlog/include
    ${3RD_PARTY_DIR}/whereami
    CACHE INTERNAL "3rdparty include directories"
)

#libs
set(3RD_PARTY_LIBS
    eigen
    spdlog
    whereami
    CACHE INTERNAL "3rdparty libraries"
)