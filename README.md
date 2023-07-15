# C++高性能服务器框架

## 1、日志模块
```c++
LogEvent
LogAppender
LogFormatter
Logger
```
日志模块基本完成，并包装了一些默认的宏方便使用，支持fmt库的格式化方式
```cpp
//使用示例
#include<foxzt/foxzt.h>

int main(){
    FOXZT_INFO("hello {}","world !");
    
    return 0;
}
//[2023-07-14 10:32:40.043] [INFO] [/path/to/test.cpp:40]: hello world !
```
## 2、配置模块
```cpp
//通过这种方式生成一个配置信息
foxzt::ConfigVar<int>::ptr g_int = foxzt::Config::Lookup<int>("system.port", 8080, "system port");
```
后续还会支持从yaml文件中读取相应的配置，例如：
```yaml
#log.yml
logs:
  - name: root
    level: info
    appenders:
      - type: FileLogAppender
        file: ./root.txt
      - type: StdoutLogAppender
  - name: system
    level: info
    appenders:
      - type: FileLogAppender
        file: ./system.txt
      - type: StdoutLogAppender
```
本项目的配置系统，遵循的规则是，约定大于配置大于编码，例如，在程序中先定义好一些配置，配置文件中的配置约定好与程序中定义的配置一样。一般来说项目的配置参数不太需要修改，但是也会有修改的时候，修改配置文件后，系统会从配置文件中读取配置内容覆盖已经定义好的配置内容，如果覆盖的配置文件中的配置没有修改，就相当于什么都没有做，如果在程序中没有实现定义的配置，即使配置文件中有也不会往程序中读入这个配置

正在逐步支持更多复杂类型的支持……