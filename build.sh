#!/bin/bash

set -e

###############################################################################
# Set you own environment here
###############################################################################
arch=$2
soc=ch32v203

###############################################################################
# function define
###############################################################################
mk_cfg()
{
        echo soc = $soc
        echo arch = $arch
        if [[ $arch == "mini" ]]
        then
                echo make config by using defconfig: ch32v203_mini_defconfig
                cfg=${soc}_${arch}_defconfig
        else
                echo "ERROR: Invalid argument for arch!"
                exit
        fi
        make $cfg
        make menuconfig
}

mk_menucfg()
{
        make menuconfig
}

mk_all()
{
        echo "make all"
        make all
}

mk_clean()
{
        echo "make clean"
        make clean
}

rv_openocd_burn()
{
        echo "make burn"
        make burn
}

rv_openocd_erase()
{
        echo "make erase"
        make erase
}

rv_openocd_reset()
{
        echo "make reset"
        make reset
}

helper()
{
        echo "---------------------------------------------------------------------"
        echo "Usage:  "
        echo "  sh build.sh [option]"
        echo "    option:"
        echo "    -m : make menuconfig"
        echo "    -p [arch]: make specified defconfig"
        echo "       supports:"
        echo "         mini: ch32v203_mini_defconfig"
        echo "    -c: make clean command"
        echo "    -d: OpenOCD swd download bin"
        echo "    -e: OpenOCD swd erase flash"
        echo "    -r: OpenOCD swd reset chip"
        echo "    -h: helper prompt"
        echo "---------------------------------------------------------------------"
}

###############################################################################
# main logic from here
###############################################################################
while getopts "mpcderh" opt; do
        case $opt in
                m)
                        mk_menucfg
                        exit
                        ;;
                p)
                        if [ -n "$arch" ]
                        then
                                mk_cfg
                        else
                                echo "ERROR: No specific deconfig found! Please enter which arch you want to make."
                                exit
                        fi
                        exit
                        ;;
                c)
                        mk_clean
                        exit
                        ;;
                d)
                        rv_openocd_burn
                        exit
                        ;;
                e)
                        rv_openocd_erase
                        exit
                        ;;
                r)
                        rv_openocd_reset
                        exit
                        ;;
                h)
                        helper
                        exit
                        ;;
                \?)
                        echo "Invalid option: -$OPTARG" >&2
                        ;;
                esac
done

mk_all
