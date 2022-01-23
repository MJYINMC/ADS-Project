$path = "./bin"
If(!(test-path $path))
{
      New-Item -ItemType Directory -Force -Path $path
}
Remove-Item ./bin/WordCount*.exe
Remove-Item ./bin/WordTree*.exe

g++ .\WordCount_stl.cpp -o $path/WordCount_stl -static
g++ .\WordCount_stl_opt.cpp -o $path/WordCount_stl_opt -static
g++ .\WordCount_avl.cpp -o $path/WordCount_avl -static
g++ .\WordCount_bin.cpp -o $path/WordCount_bin -static
g++ .\WordTree.cpp -o $path/WordTree -static
g++ .\WordTree_multithread.cpp -o $path/WordTree_multithread -pthread -static

# Compiler Info:
# Thread model: posix
# gcc version 8.1.0 (x86_64-posix-seh-rev0, Built by MinGW-W64 project)

# Platform Info:
# CPU: AMD Ryzen 7 5800X 8-Core Processor 3.80 GHz 
# (@100MHzx50 single-core 
# @100Mhzx46.5 multi-core)
# RAM: 16.0 GB 3866Mhz C15-15-15-28
# DISK: SN550 1T SSD

# System Info:
# Windows 10 Pro for WorkStations
# 20H2 19042.867