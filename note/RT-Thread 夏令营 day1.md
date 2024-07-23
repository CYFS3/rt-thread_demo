# RT-Thread 夏令营 day1

## 零、准备工作

电脑环境：Windows11

开发板：RT-Spark（星火一号）

软件安装包：[百度网盘]()

* Env：2.0.0==（版本需要2.0.0以上）==
* Openocd
* Vscode
* MobaXterm

## 一、环境的安装

### 1.env安装

下载网盘中的`env-windows-v2.0.0.zip`

![image-20240722174935157](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140724.png)

把压缩包放在自己想安装的目录下，进行解压。我这边放在D盘的目录下。

![image-20240722173914192](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140726.png)

进行解压，随后进入解压后的路径，运行env.exe，如果 出现弹窗点击确定即可。

![image-20240722174105441](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140727.png)

![d0c25fdd0b752de40e9a813fcd7ddd8](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140728.jpg)

因为我们下载的是在线版本，所以一开始他会去安装一个虚拟的环境，受网络的影响，可能会下载失败。

![AgAABiCCFDzvQahLVnVGiJcx3TmPQMEj](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140729.png)

**解决方法如下**

* 方法1：删除env目录下的`.venv`,并重新打开`env.exe`

![image-20240722174801182](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140730.png)

* 如果方法1还是一样的话，请下载网盘中的另一个打包好的env工具`env-windows-v2.0.0-venv.zip`

  ![image-20240722175101156](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140731.png)

  ==如果下载这个的话路径必须解压到C盘根目录，结构为：C:\env-windows\env.exe...==

**将env工具添加进右键**

打开env.exe，右击空白处，进入设置。

![image-20240722175506859](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140732.png)

![image-20240722175859606](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140733.png)



此时可以看到右键之后就有env了。

![image-20240722175945322](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140734.png)

### 2.Openocd

直接下载直接解压即可，如果想在终端里面使用可以添加环境变量。

![image-20240722195040467](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140735.png)

openocd是一款开源的开放式片上调试软件。

### 3.MobaXterm

下载安装即可

### 4.Vscode

下载安装，注意在安装的过程中可以，把这两个选项勾上，方便之后的操作，如果已经安装的，右键没有opencv code 的话，下面我提供一种解决方式。

![063e86741a0a935a98a739c9762b7b7](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140736.jpg)



~~~bash
Windows Registry Editor Version 5.00

[HKEY_CLASSES_ROOT\*\shell\VSCode]
@="Open with Code"
"Icon"="D:\\Microsoft VS Code\\Code.exe"

[HKEY_CLASSES_ROOT\*\shell\VSCode\command]
@="\"D:\\Microsoft VS Code\\Code.exe\" \"%1\""

Windows Registry Editor Version 5.00

[HKEY_CLASSES_ROOT\Directory\shell\VSCode]
@="Open with Code"
"Icon"="D:\\Microsoft VS Code\\Code.exe"

[HKEY_CLASSES_ROOT\Directory\shell\VSCode\command]
@="\"D:\\Microsoft VS Code\\Code.exe\" \"%V\""

Windows Registry Editor Version 5.00

[HKEY_CLASSES_ROOT\Directory\Background\shell\VSCode]
@="Open with Code"
"Icon"="D:\\Microsoft VS Code\\Code.exe"

[HKEY_CLASSES_ROOT\Directory\Background\shell\VSCode\command]
@="\"D:\\Microsoft VS Code\\Code.exe\" \"%V\""

~~~

新建一个txt文本，然后把其中的路径换成自己vscode的安装路径，最后把文件重新命名为xxx.reg，然后打开即可。

**插件的安装**

* C/C++
* C/C++ Extension Pack
* Cortex-Debug
* Git Graph

![image-20240722200429222](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140737.png)

![image-20240722200439395](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140738.png)

![image-20240722200447249](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140739.png)

### 5.Git

Git是一个免费的、开源的**分布式版本控制系统**，可以高速处理从小型到大型的各种项目。

安装参考：http://t.csdnimg.cn/cGnfU

安装完成之后，右键查看有没有Git bash以及GUI





最后所有软件安装成功之后，右键就可以看到

![image-20240722201111273](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140740.png)

（笔者把Git GUI的注册表删了，所以少了一个）

## 二、Git的使用

右键打开Git bash

### 1.用户名以及邮箱的配置

初次使用git需要设置你的用户名以及邮箱，这将作为当前机器git的标识，如果你用它来下载远程仓库一些需要登录权限的仓库会要求登录，git默认使用配置邮箱以及用户名登入，但会要求你手动输入密码。

~~~bash
git config --global user.name "你的用户名"
git config --global user.email "你的邮箱"
~~~

==上面的用户名和邮箱必须与远程服务（github、gitee）注册的一致。==

~~~bash
git config --global -l
~~~

查看是否配置成功。

### 2.克隆rt-thread仓库

~~~bash
git clone https://github.com/RT-Thread/rt-thread.git
~~~

==github国内用户访问困难，需要代理==，或者可以使用开源项目`fistgithub`(百度网盘也有提供)

也可以克隆gitee的rt-thread仓库。

~~~bash
git clone https://gitee.com/rtthread/rt-thread.git
~~~

进入到rt-thread的目录下，切换分支`RSOC-2024`

~~~bash
cd rt-thread
//github
git checkout RSOC-2024
//gitee
git checkout -b RSOC-2024
~~~

## 三、打包编译星火一号BSP

### 1.打包

进入bsp的目录下，星火一号采用的MCU为STM32F407。

![image-20240722204516920](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140741.png)

右键打开env。

打包项目

~~~bash
scons --dist
~~~

![image-20240722204638799](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140742.png)

打包成功，后面的提示是说我们打包的时候可以加要构建的目标，所以可以使用下面进行打包

~~~bash
scons --dit --target=vsc
~~~

![image-20240722204849000](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140743.png)

可以看到目录下多出了dist文件夹，文件夹中的project是可以在任意位置使用的，这样做的目的是为了不把rt-thread的仓库弄乱以及方便开发。

### 2.编译

我们可以把上面打包得到的文件，移动至自己的工作区。

然后再打开env工具，进行编译

~~~bash
scons
~~~

![image-20240722205238702](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140744.png)

可以看到编译成功得到xxx.elf文件

## 四、Vscode 下载调试

可以直接在env下`code .`

如果打不开的话应该是没有设置环境变量

打开自己vscode 的安装路径下的bin，复制路径添加环境变量即可（不会的百度）。

![image-20240722210952641](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140745.png)

### 1.添加openocd

点击设置，搜索setting.json

![image-20240722211422700](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140746.png)

![image-20240722211444560](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140747.png)

![image-20240722211512194](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140748.png)

添加自己的openocd路径即可

### 2.调试

点击调试，创建

![image-20240722211725666](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140749.png)

![image-20240722212024684](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140750.png)

将文件中的东西替换成下面的。

~~~bash
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "rt-spark-openocd",
            "executable": "${workspaceRoot}/rt-thread.elf",
            "request": "launch",
            "type": "cortex-debug",
            "runToEntryPoint": "main",
            "targetId": "STM32F407ZG",
            "servertype": "openocd",
            "configFiles": [
                "interface/stlink-v2.cfg",
                "target/stm32f4x.cfg"
            ],
            "armToolchainPath": "D:/env-windows/tools/gnu_gcc/arm_gcc/mingw/bin", // ！！！需要修改为自己的GCC 工具链路径 ！！！
            "gdbPath": "D:/env-windows/tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-gdb.exe" // ！！！需要修改为自己的GDB 路径 ！！！
        }
    ]
}

~~~

==注意：路径需要改成自己env安装的路径==

连接开发板，需要连续debug口，星火一号板载了st-link。

点击F5就可以进行调试代码了。

![image-20240722212742019](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140751.png)

其他的调试按钮可以自己去了解一下。

### 3.下载

如果想直接下载，可以使用终端命令行

打开一个新的终端，使用openocd进行下载

~~~bash
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program rt-thread.elf verify reset; shutdown"
~~~

## 五、使用MobaXterm查看FinSH控制台输出

打开MobaXterm，点击Session。

![image-20240722213738096](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140752.png)

![image-20240722213839684](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140753.png)

按下开发板的reset，或者在MobaXterm输入reboot，就可以看到rt-thread的欢迎界面。

![image-20240722214017214](https://raw.githubusercontent.com/CYFS3/Typroa/main/202407222140754.png)