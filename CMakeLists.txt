cmake_minimum_required(VERSION 3.16)
project(HappyWorld CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 增量编译优化
set(CMAKE_SKIP_RULE_DEPENDS ON)
set(CMAKE_EXPERIMENTAL_PCH ON)
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY ON)

set(APP_NAME "HappyWorld")

# 设置 cocos2d-x 根目录
set(COCOS2DX_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cocos2d-x" CACHE PATH "Cocos2d-x root directory")

# 使用 cocos2d-x 的 CMake 模块
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)
include(CocosBuildSet)

# 添加 cocos2d-x 引擎
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)

# 源文件
file(GLOB_RECURSE GAME_SOURCES "Classes/*.cpp" "Classes/*.h")

# 创建可执行文件
add_executable(${APP_NAME} WIN32 ${GAME_SOURCES})

# 链接 cocos2d 库
target_link_libraries(${APP_NAME} cocos2d)

# 添加项目包含路径
target_include_directories(${APP_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Classes)

# 使用 cocos2d 的配置函数
setup_cocos_app_config(${APP_NAME})

# 设置输出到 install 目录
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install")
set_target_properties(${APP_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

# 解决中文 UTF-8 源文件编译问题
target_compile_options(${APP_NAME} PRIVATE /utf-8)

# 复制 DLL 到 install 目录
cocos_copy_target_dll(${APP_NAME})

# 复制资源
cocos_get_resource_path(APP_RES_DIR ${APP_NAME})
cocos_copy_target_res(${APP_NAME} LINK_TO ${APP_RES_DIR} FOLDERS "${CMAKE_CURRENT_SOURCE_DIR}/Resources")

# Install 配置
install(TARGETS ${APP_NAME} RUNTIME DESTINATION .)
install(DIRECTORY Resources/ DESTINATION Resources)

