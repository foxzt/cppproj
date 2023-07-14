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
程序在启动的时候会加载yml文件中的配置，生成两个日志器。
功能模块正在开发中……