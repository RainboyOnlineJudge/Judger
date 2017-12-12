#Judger [![Build Status](https://travis-ci.org/QingdaoU/Judger.svg?branch=newnew)](https://travis-ci.org/QingdaoU/Judger)

Judger for OnlineJudge 

[Document](http://docs.onlinejudge.me/Judger/)

## 说明

`rainboy`在原`Judger`的基础上的魔改版本


## 安装

```
sudo apt-get install libseccomp-dev
mkdir build && cd build && cmake .. && make && sudo make install
```

```
cd ./bindings/python-rj/
sudo python3 setup.py install
```

## 使用


```python
import _judger

ret = _judger.run(max_cpu_time=1000,
                  max_real_time=2000,
                  max_memory=128 * 1024 * 1024,
                  max_process_number=200,
                  max_output_size=10000,
                  max_stack=32 * 1024 * 1024,
                  # five args above can be _judger.UNLIMITED
                  exe_path="main",
                  input_path="1.in",
                  output_path="/dev/stdout",
                  error_path="/dev/stderr",
                  args=[],
                  # can be empty list
                  env=[],
                  log_path="judger.log",
                  # can be None
                  seccomp_rule_name="c_cpp",
                  uid=0,
                  gid=0,
                  inf=False,        # 以更详细的输出结果
                  debug=False       # 运行时候 输出debug 信息
                  )
```

## 返回值

```
{
    'result':    运行结果,
    'cpu_time':  运行时间 ms
    'real_time': 真实时间 ms
    'memory':    占用内存 byte
    'signal':    退出信号
    'exit_code': 退出码
    'error':     错误码
}
```

`result`的含义

```
WRONG_ANSWER (judger module will never return this value, it's used for awswer checker)
SUCCESS = 0 (this only means the process exited normally)
CPU_TIME_LIMIT_EXCEEDED = 1
REAL_TIME_LIMIT_EXCEEDED = 2
MEMORY_LIMIT_EXCEEDED = 3
RUNTIME_ERROR = 4
SYSTEM_ERROR = 5
```



`error`的含义

```
SUCCESS = 0
INVALID_CONFIG = -1
FORK_FAILED = -2
PTHREAD_FAILED = -3
WAIT_FAILED = -4
ROOT_REQUIRED = -5
LOAD_SECCOMP_FAILED = -6
SETRLIMIT_FAILED = -7
DUP2_FAILED = -8
SETUID_FAILED = -9
EXECVE_FAILED = -10
SPJ_ERROR = -11 (judger module will never return this value, it's used for awswer checker)')
```
