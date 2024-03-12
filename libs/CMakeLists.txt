add_subdirectory(imgui)
add_subdirectory(glm)


target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
    imgui/imgui
    glm/
)