# ch32v203_mini

> 如果那个人宠你，领不领证都没关系。如果那个人不爱你，法律也保护不了你，因为法律拴不住人心。<br>
> <br>
> 结婚不等于幸福，单身也不意味着不幸福。<br>
> 我们出生就是一个人，最后也不可能同时离去。<br>
> 爱情是精神的奢侈品，没有也行！<br>
***
##  **Document Download**
1. Download [RISC-V Technical Specifications](https://wiki.riscv.org/display/HOME/RISC-V+Technical+Specifications).
2. Download [WCH沁恒 ch32v203相关文档](https://www.wch.cn/search?q=ch32v203&t=downloads)

## **Compilation environment & Commit message config**
1. Download [Toolchain and debug tools](http://www.mounriver.com/download). The compilation toolchain and OpenOCD tools are downloaded from **_MounRiver Studio_**.
2. Run [tools/toolchain/toolchain_cfg.sh](./tools/toolchain/toolchain_cfg.sh)
3. If you submit code for the first time, run [tools/githook/git_msg_set.sh](./tools/githook/git_msg_set.sh). Otherwise, skip this step.
4. Run [./build.sh -m mini](./build.sh), create default congfig file.
5. Run [./build.sh](./build.sh), complete compilation.

## **OpenOCD Debug & Download program**
1. Run [./build.sh -d](./build.sh), Download image & Verify image.
1. Run [./build.sh -e](./build.sh), Erase flash.
