BASE_PATH=`pwd`
CMD_PATH=`pwd`/demo_test_py
if [ ! -d $CMD_PATH ]; then
  ./compile_py.sh
fi

cd $BASE_PATH
cp "1.in" $CMD_PATH

/judge/qjudge --ct=1000 \
--rt=`expr 3 \* 1000` \
--ml=`expr 128 \* 1024 \* 1024` \
--sl=`expr 64 \* 1024 \* 1024` \
--pn=1 \
--ol=`expr 64 \* 1024 \* 1024` \
--exe=/usr/bin/python3 \
--in=$CMD_PATH/1.in \
--out=$CMD_PATH/1.out \
--err=$CMD_PATH/err \
--log=$CMD_PATH/log \
--arg=-E,-B,$CMD_PATH/main.py \
--env=PATH=/usr/bin \
--sec=general \
--gid=-1 \
--uid=-1 \
--inf \
--debug
