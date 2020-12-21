皮肤说明文档
============

### 版本

文档版本：V0.1

### 目录结构

目录结构如下：

skin/
  |
  |-black/
  |-black.qss
  |
  |-blue/
  |-blue-skin.qss
  |
  |-default/
  |-default-skin.qss

其中，blue-skin.qss default-skin.qss为系统皮肤文件，black.qss为用户自己定义皮肤文件。

### 自定义皮肤

若用户想制作自己的个性皮肤，可参考black.qss。运行程序前，将自制皮肤文件复制到 ~/.local/share/chinese-calendar/skin 目录（若无此目录，请使用 mkdir -p ~/.local/share/chinese-calendar/skin 来新建）下即可换肤。

操作如下：

$ mkdir -p ~/.local/share/chinese-calendar/skin
$ cd ~/.local/share/chinese-calendar/skin
$ cp -r /usr/share/chinese-calendar/skin/black .
$ cp /usr/share/chinese-calendar/skin/black.qss .
$ sed -i "s:%HOME%:`echo $HOME`:" black.qss

### 问题与反馈

mutse(默之)	<yyhoo2.young@gmail.com>
 
