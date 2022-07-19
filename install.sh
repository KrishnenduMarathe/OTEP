#! /bin/bash

install_path=$(pwd)

# Update Repo
git pull
make -B

cd ~/.local/share/applications/
if [[ -f otep.desktop ]]
then
    rm otep.desktop
fi

echo "[Desktop Entry]
Encoding=UTF-8
Path=$install_path
Version=\`cat $install_path/config/version\`
Type=Application
Terminal=false
Exec=\"$install_path/Otep\"
Name=OTEP
Comment=Ouroboros Terminal Emulation Program
Category=Utility;
Icon=$install_path/config/icon.png" > otep.desktop
cd $install_path