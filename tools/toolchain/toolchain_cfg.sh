#!/bin/bash

cd beforeinstall && ./start.sh && cd ../

toolchain_dir=$(cd $(dirname $0);pwd)

echo "export PATH=\$PATH:$toolchain_dir/RISC-V_Embedded_GCC/bin/" >> ~/.bashrc
