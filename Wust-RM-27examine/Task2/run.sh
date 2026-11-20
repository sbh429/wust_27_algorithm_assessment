#!/bin/bash
build() {
    if [ ! -f app ] || [ main.cpp -nt app ]; then
        g++ main.cpp -o app
        if [ $? -ne 0 ]; then
            echo "编译失败" >&2
            exit 1
        fi
    else
        echo "已是最新可执行文件"
    fi
}
if [ "$1" == "build" ]; then
    build
elif [ "$1" == "run" ]; then
    build
    if [ "$2" == "--" ]; then
        ./app "$3" "$4"
    else
        ./app
    fi
elif [ "$1" == "clean" ]; then
    rm -f app
elif [ "$1" == "help" ]; then
    echo "Usage: $0 [build(编译项目)|run(运行项目)|clean(清除编译产物)|help(帮助信息)]"
else
    echo "未知命令： $1" >&2
    exit 1
fi