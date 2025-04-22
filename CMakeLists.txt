cmake_minimum_required(VERSION 3.10)

project(hash_table)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 设置最严格的编译选项
if(MSVC)
    # MSVC编译器的严格选项
    add_compile_options(
        /W4     # 启用最高级别警告
        /WX     # 将警告视为错误
        /permissive-  # 严格标准遵循
        /analyze # 启用代码分析
        /D_CRT_SECURE_NO_WARNINGS # 避免安全函数警告
        /guard:cf # 启用Control Flow Guard
        /sdl     # 启用更多安全检查
        /utf-8   # 使用UTF-8编码
        /Zc:__cplusplus # 正确报告C++标准
        /diagnostics:caret # 改进的诊断信息
    )
elseif(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    # GCC/Clang编译器的严格选项
    add_compile_options(
        -Wall           # 启用所有常见警告
        -Wextra         # 启用额外警告
        -Werror         # 将警告视为错误
        -pedantic       # 严格遵循标准
        -pedantic-errors # 将标准违规视为错误
        -Wconversion    # 类型转换警告
        # -Wshadow        # 变量覆盖警告
        -Wundef         # 未定义标识符警告
        -Wcast-align    # 指针对齐警告
        -Wstrict-prototypes # 函数原型警告
        -Wstrict-overflow=5 # 严格溢出检查
        -Wformat=2      # 格式字符串检查
        -Wformat-security # 格式字符串安全检查
        -Wunreachable-code # 不可达代码警告
        -Wdouble-promotion # float提升为double警告
        -Wfloat-equal   # 浮点数相等比较警告
    )
    
    # 添加安全选项
    add_compile_options(
        -fstack-protector-strong # 栈保护
        -D_FORTIFY_SOURCE=2     # 缓冲区溢出检查
    )
    
    # 添加调试选项
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        # 检查是否为Windows平台
        if(NOT WIN32)
            # 在Debug模式下启用sanitizer（仅限非Windows平台）
            add_compile_options(
                -fsanitize=address,undefined # 地址和未定义行为检查
                -fno-omit-frame-pointer    # 保留栈帧指针以便更好的调试
            )
            # 链接选项也需要添加sanitizer
            add_link_options(
                -fsanitize=address,undefined
            )
        else()
            # Windows平台仅保留栈帧指针
            add_compile_options(
                -fno-omit-frame-pointer    # 保留栈帧指针以便更好的调试
            )
            message(STATUS "Address Sanitizer disabled on Windows platform")
        endif()
    endif()
endif()

add_library(hash_table STATIC
    hash_table.c
    hash_table.h
    utility.c
    memcheck.c
)

# 如果需要生成可执行文件测试，可以取消以下注释
add_executable(hash_table_test test.c)
target_link_libraries(hash_table_test hash_table)

# 添加memcheck测试可执行文件
add_executable(memcheck_test memcheck_test.c)
target_link_libraries(memcheck_test hash_table)

# 添加迭代器测试可执行文件
add_executable(iterator_test iterator_test.c)
target_link_libraries(iterator_test hash_table)

# 添加迭代器中删除键值对测试可执行文件
add_executable(iterator_remove_pair remove_current_test.c)
target_link_libraries(iterator_remove_pair hash_table)
