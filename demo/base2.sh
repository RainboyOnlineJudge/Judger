BASE_PATH=`pwd`
CMD_PATH=`pwd`/demo_test_compile
if [ ! -d $CMD_PATH ]; then
  ./compile.sh
fi

cd $BASE_PATH
cp "1.in" $CMD_PATH

/judge/qjudge --ct=1000 \
--rt=`expr 3 \* 1000` \
--ml=`expr 128 \* 1024 \* 1024` \
--sl=`expr 64 \* 1024 \* 1024` \
--pn=1 \
--ol=`expr 64 \* 1024 \* 1024` \
--exe=$CMD_PATH/main \
--in=$CMD_PATH/1.in \
--out=$CMD_PATH/1.out \
--err=$CMD_PATH/err \
--log=$CMD_PATH/log \
--sec=c_cpp \
--gid=-1 \
--uid=-1 \
--inf \
--debug
