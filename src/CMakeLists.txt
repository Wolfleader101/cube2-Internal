# add_subdirectory(Logger)
add_subdirectory(Math)
add_subdirectory(Renderer)
add_subdirectory(Game)
add_subdirectory(Internal)

SET(SOURCES
    # ${LOGGER_SRC}
    ${MATH_SRC}
    ${RENDERER_SRC}
    # ${GAME_SRC}
    ${INTERNAL_SRC}
    src/dllmain.cpp
    PARENT_SCOPE
)


SET(GLOBAL_INCLUDE_DIRS
    # ${LOGGER_INCLUDE_DIR}
    ${MATH_INCLUDE_DIR}
    ${RENDERER_INCLUDE_DIR}
    ${GAME_INCLUDE_DIR}
    ${INTERNAL_INCLUDE_DIR}
    PARENT_SCOPE
)
