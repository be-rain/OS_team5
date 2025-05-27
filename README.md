# Operating-System-TEAM-5

### build

<pre>make</pre>

### clean

<pre>make clean</pre>

### fclean

<pre>make fclean</pre>

### re

<pre>make re</pre>

### run

<pre>./TEAM5_LINUX</pre>

### How to handling segmentation fault

<pre>gcc [all source files] -fsanitize=address -g3 -o [output]</pre>

### If you want to build faster

<pre>make -j</pre>

### In Ubuntu

<pre>sudo apt install git
git clone https://github.com/sayyyho/sample-linux
sudo apt install make
sudo apt install gcc</pre>

### 로컬에서

<pre>
git clone https://github.com/Hello-Worldismine/OS_team5.git
cd OS_team5
git checkout seho-park
make
# 에러 뜨면 make clean 후 다시 make
./TEAM5_LINUX
</pre>
