# coding:utf-8

import subprocess

UNLIMITED=-1
RESULT_SUCCESS=0
RESULT_CPU_TIME_LIMIT_EXCEEDED=1
RESULT_REAL_TIME_LIMIT_EXCEEDED=2
RESULT_MEMORY_LIMIT_EXCEEDED=3
RESULT_RUNTIME_ERROR=4
RESULT_SYSTEM_ERROR=5

def check_unlimited(val):
    if val <=0:
        return -1
    else:
        return val

def run_program_args(
        ct=1,   # cpu_time_limit
        rt=None,   # real_time_limt
        ml=128*1024*1024, # memory_limit
        sl=1024*1024*1024, # stack limit
        pn=-1,   # process_number default unlimited
        ol=128*1024*1024, # output_limit
        exe=None,
        _in="/dev/null",
        out="/dev/null",
        err="/dev/null",
        arg=[],
        env=[],
        log="/dev/null",
        sec=None,
        gid=-1,
        uid=-1,
        inf=False,
        debug=False
        ):
    cmdline = ''

    cmdline += '--ct='+str(check_unlimited(ct))+' '

    if rt:
        cmdline += '--rt='+str(check_unlimited(5*ct))+' ' # 默认是ct的5倍
    else:
        cmdline += '--rt=-1 '

    cmdline += '--ml='+str(check_unlimited(ml))+' '
    cmdline += '--ol='+str(check_unlimited(ol))+' '
    cmdline += '--sl='+str(check_unlimited(sl))+' '
    cmdline += '--exe='+exe+ ' '
    cmdline += '--in='+str(_in)+' '
    cmdline += '--out='+str(out)+' '
    cmdline += '--err='+str(err)+' '
    cmdline += '--pn='+str(pn)+' '

    

    if sec :
        cmdline += '--sec='+sec+' '

    cmdline += '--log='+log+' '


    cmdline += '--arg='+','.join(arg)+ ' '
    cmdline += '--env='+','.join(env)+ ' '
    cmdline += '--gid='+str(gid)+' '
    cmdline += '--uid='+str(uid)+' '

    if inf:
        cmdline += '--inf '

    if debug:
        cmdline += '--debug '

    return cmdline

def check_args(**kwargs):
    valid_args = [ "max_cpu_time", "max_real_time", "max_memory", "max_process_number", "max_output_size", "max_stack", "exe_path", "input_path", "output_path", "error_path", "args", "env", "log_path", "seccomp_rule_name", "uid", "gid", "inf","debug"]

    args_map={ "max_cpu_time":"ct",
            "max_real_time":"rt",
            "max_memory":"ml",
            "max_process_number":"pn",
            "max_output_size":"ol",
            "max_stack":"sl",
            "exe_path":"exe",
            "input_path":"_in",
            "output_path":"out",
            "error_path":"err",
            "args":"arg",
            "env":"env",
            "log_path":"log",
            "seccomp_rule_name":"sec",
            "uid":"uid",
            "gid":"gid",
            "inf":"inf",
            "debug":"debug" }

    __args = {}
    # 为空
    if not kwargs:
        raise ValueError("Invalid args and kwargs")

    for key in kwargs:
        if not key in valid_args:
            raise ValueError("Invalid args and kwargs")
        elif key == 'args':
            if not isinstance(kwargs[key], list):
                raise ValueError("args must be a list")
            for item in kwargs[key]:
                if not isinstance(item,str):
                    raise ValueError("arg item must be a string")
        elif key == 'env':
            if not isinstance(kwargs[key], list):
                raise ValueError("env must be a list")
            for item in kwargs[key]:
                if not isinstance(item,str):
                    raise ValueError("env item must be a string")
        __args[ args_map[key] ] = kwargs[key]

    return __args


def run(**kwargs):
    # print(kwargs)
    args = check_args(**kwargs)
    # print(args)
    _cmdline = (run_program_args(**args))
    _cmdline = '/judge/qjudge ' + _cmdline
    cmd  = _cmdline.split(' ')

    if cmd[-1] == "":
        cmd.pop(-1)

    # print(cmd)
    # print(cmd)
    p=subprocess.Popen(cmd,stdout=subprocess.PIPE)
    out,err = p.communicate()
    _out = out.decode('ASCII').split(' ')
    return {
            'result':    int(_out[0]),
            'cpu_time':  int(_out[1]),
            'real_time': int(_out[2]),
            'memory':    int(_out[3]),
            'signal':    int(_out[4]),
            'exit_code': int(_out[6]),
            'error':     int(_out[5]),
            }

if __name__ == '__main__':
    pass

