#### 操作说明
对于sim_root文件，将其放在主目录下即可。
打开终端,执行
```shell
mkdir build
cd build
cmake ..
make 
cd bin
./main 任意数
```
程序运行时，只需输入对应操作前面的数字即可

根据想要的操作，配置对应的配置文件

配置完成后，选择修改查询数据中的1，根据配置文件创建目录及文件

对于修改查询操作(对gpio与adc文件)：

1.输入对应的操作

2.输入 inx_input，gpiox选择文件，x可根据显示信息选择

3.如果修改，输入相要写的字符，查询，忽略此步

对于回放操作(对can与usart文件)：

根据指定的路径，回放相应的文件