CMD_PATH=`pwd`/demo_test_py
if [ ! -d $CMD_PATH ]; then
    mkdir $CMD_PATH
    echo "create "$CMD_PATH
fi

cp "main.py" $CMD_PATH
cd $CMD_PATH
/judge/qjudge --ct=-1 \
--rt=-1 \
--ml=-1 \
--sl=10240000000 \
--pn=1 \
--ol=1024000 \
--exe=/usr/bin/python3 \
--in=/dev/null \
--out=compile_out \
--err=compile_error \
--log=compile_log \
--arg=-m,py_compile,main.py \
--env=PATH=/usr/bin \
--gid=-1 \
--uid=-1 \
--debug \
--inf
