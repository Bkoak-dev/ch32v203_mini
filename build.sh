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
}

mk_menucfg()
{
        mk_cfg
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

helper()
{
        echo "---------------------------------------------------------------------"
        echo "Usage:  "
        echo "  sh build.sh [option]"
        echo "    option:"
        echo "    -m [arch]: make menuconfig by specified defconfig"
        echo "	     supports:"
        echo "         v3: ch32v203_mini_defconfig"
        echo "    -p [arch]: make specified defconfig"
	echo "       supports:"
	echo "         v3: ch32v203_mini_defconfig"
        echo "    -c: make clean command"
        echo "    -h: helper prompt"
        echo "---------------------------------------------------------------------"
}

###############################################################################
# main logic from here
###############################################################################
while getopts "mpcdolgah" opt; do
        case $opt in
                m)
                        if [ -n "$arch" ]
                        then
                                mk_menucfg
                        else
                                echo "ERROR: No specific deconfig found! Please enter which arch you want to make."
                                exit
                        fi
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
