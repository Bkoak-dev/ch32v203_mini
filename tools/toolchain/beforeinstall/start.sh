#! /bin/bash
echo  -e "\nCopy Libs"
sudo cp -P ./lib*    /usr/lib
echo  -e "Register new Libs"
sudo ldconfig
echo "copy rules"
sudo cp ./50-wch.rules /etc/udev/rules.d
sudo cp ./60-openocd.rules  /etc/udev/rules.d
echo "Reload rules"
sudo udevadm control  --reload-rules
echo -e "DONE"

