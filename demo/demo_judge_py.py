import _judger
import os

basedir = os.path.split(os.path.realpath(__file__))[0]


rundir = basedir+'/demo_test_py'
exe_path = rundir+'/main.py'


if os.path.exists(rundir) == False:
    os.mkdir(rundir)

os.system("cp 1.in ./demo_test_py")
os.system("cp main.py ./demo_test_py")

if os.system(" ".join(["python3","-m","py_compile",exe_path])) !=0:
    print('编译失败')
    exit(1)


ret = _judger.run(max_cpu_time=1000,
                  max_real_time=2000,
                  max_memory=128 * 1024 * 1024,
                  max_process_number=200,
                  max_output_size=10000,
                  max_stack=32 * 1024 * 1024,
                  # five args above can be _judger.UNLIMITED
                  exe_path="/usr/bin/python3",
                  input_path=rundir+"/1.in",
                  output_path=rundir+"/1.out",
                  error_path="/dev/stderr",
                  args=["-E","-B",exe_path],
                  # can be empty list
                  env=[],
                  log_path=rundir+"/log",
                  # can be None
                  seccomp_rule_name="general",
                  uid=0,
                  gid=0)
print(ret)
