#!/bin/bash
debugMode=false
#是否开始debug模式
for arg in "$@"; do
    if [ "$arg" = "--debug" ]; then
        debugMode=true
        break
    fi
done

home=$yhz_home/cqpgsim
ibm_data=$home/data_in/ibm_data
thu_data=$home/data_in/thu_data
data_out=$home/data_out

#删除先前运行结果
cd $data_out
prefix=${1%%.*}  # 获取 $1 的前缀部分
# 获取与前缀匹配的文件列表
file_list=( ${prefix}* )

if [ ${#file_list[@]} -gt 0 ]; then
    for file in "${file_list[@]}"; do
        rm -f "$file"
        echo "文件 $file 已删除"
    done
else
    echo "frist run "
fi

#配置运行文件
cd $home/bin
if [[ "${1:0:3}" == "ibm" ]]; then
    # 处理以 "ibm" 开头的路径
    # 在这里编写针对 ibm 的路径处理代码
    # 可以使用 $1 来引用传入的变量值
    echo " ibm "
    spice=$ibm_data/$1/$1.spice
    ofile=$data_out/$1.txt 
    solution=$ibm_data/$1/$1.solution
    ana=$data_out/$1.ana.txt
    
elif [[ "${1:0:3}" == "thu" ]]; then
    # 处理以 "thu" 开头的路径
    # 在这里编写针对 thu 的路径处理代码
    # 可以使用 $1 来引用传入的变量值
    echo " thu "
    spice=$thu_data/$1/$1.spice
    ofile=$data_out/$1.txt 
    solution=$thu_data/$1/$1.solution
    ana=$data_out/$1.ana.txt
else
    # 其他情况的处理
    echo "路径不满足条件"
fi
start_time=$(date +%s.%N)
if [ "$debugMode" = true ]; then
    # 调试模式下的代码块
    echo "调试模式已启用"
    ./cqpgsim $spice  $ofile >> $ana &
else
    echo "调试模式未启用"
    ./cqpgsim $spice $ofile $solution >> $ana &
fi



# 获取cqpgsim程序的进程ID
pid=$!

# 初始化最大内存使用为0
max_memory=0
mem_usage=0
# 循环检测cqpgsim程序的内存使用
while true; do
    # 检测cqpgsim程序是否已结束
    if ! ps -p $pid > /dev/null; then
        break
    fi
    # 使用ps命令获取cqpgsim程序的内存使用数据
    mem_info=$(ps -p $pid -o rss=)
    # 提取内存使用量
    mem_usage=$(echo "$mem_info" | awk '{print $1}')
    # 更新最大内存使用
    if [ $mem_usage -gt $max_memory ]; then
        max_memory=$mem_usage
    fi
    # 休眠一段时间，以控制检测频率
    sleep 1
done

end_time=$(date +%s.%N)
runtime=$(echo "$end_time - $start_time" | bc)
echo "shell 判断程序总运行时间：$runtime 秒">> $ana
max_memory_mb=$((max_memory / 1024))

# 输出峰值内存使用
# echo "cqpgsim程序峰值内存使用：$max_memory KB"
echo "cqpgsim程序峰值内存使用：$max_memory_mb MB" >>  $ana