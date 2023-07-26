# C++高性能服务器框架

开发进度

|    模块名称     | 开发进度 |    日期     |  
|:-----------:|:----:|:---------:|
|    日志模块     | 基本完成 | 2023.6.20 |
|    配置模块     | 基本完成 | 2023.7.6  |
|    线程模块     | 基本完成 | 2023.7.8  |
| 日志与配置配置线程安全 | 基本完成 | 2023.7.19 |
|    协程模块     | 刚开始  | 2023.7.26 |
|     ...     | ...  |    ...    |

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
#include
<foxzt/foxzt.h>

int main(){
FOXZT_INFO("hello {}", "world !");

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

本项目的配置系统，遵循的原则是，约定大于配置大于编码  
目前日志系统已经与配置系统整合完毕，在log.yml文件中可以读取日志器的配置

下一步准备将线程安全部分一起整合到日志系统与配置系统中，使日志系统与配置系统是线程安全的

# 3、线程模块

线程本来打算使用POSIX提供的API封装，但是C++17好像提供了读写锁、lock_guard等的实现，  
所以，简单使用C++的原子操作封装了一个自旋锁。虽然没有用上，但是还是为之后的拓展保留。

# 4、协程模块

开发协程模块前，对assert函数进行了包装，使其可以打印出更加详细的信息，可以打印出栈帧等信息，方便开发时debug。
参考了各种协程库的实现，包括libco、libgo、还有boost中的协程实现，这些工业级别的协程库太过复杂，  
打算简单使用ucontext实现一个协程模块，只支持从主协程到子协程，不支持子协程间任意切换，子协程执行完后或者被中断需要将控制权还给主协程。  
目前正在开发中……

