# !/bin/bash

cd /media/liuyi/Linux/github/picture
git pull

cd /home/liuyi/Desktop
mv $1 /media/liuyi/Linux/github/picture/pictures

cd /media/liuyi/Linux/github/picture
git add .
git commit -m "picture"
git push origin master

echo -e "\n"
echo "![](https://raw.githubusercontent.com/liuyi12138/picture/master/pictures/$1)"
echo -e "\n"