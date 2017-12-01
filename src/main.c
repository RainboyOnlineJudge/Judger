#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <string.h>
#include "runner.h"


typedef unsigned char bool;

//存args
char _args[255][255]={0};
char _env[255][255]={0};

char default_seccomp[2][10]={"c_cpp","general"};

char _result_msg[][30] = {
    "WRONG_ANSWER",
    "ACCEPT",
    "CPU_TIME_LIMIT_EXCEEDED",
    "REAL_TIME_LIMIT_EXCEEDED",
    "MEMORY_LIMIT_EXCEEDED",
    "RUNTIME_ERROR",
    "SYSTEM_ERROR",
};

// judge 参数
struct config judge_config;

struct result _result;

struct _args_stat {
    bool is_set_ct;
    bool is_set_rt;
    bool is_set_ml;
    bool is_set_sl;
    bool is_set_pn;
    bool is_set_ol;
    bool is_set_exe;
    bool is_set_in;
    bool is_set_out;
    bool is_set_err;
    bool is_set_arg;
    bool is_set_env;
    bool is_set_log;
    bool is_set_sec;
    bool is_set_gid;
    bool is_set_uid;
    bool is_set_moreinfo;
    bool is_set_debug;
}args_stat;

/* ------------------  argp  ------------------------ */
const char *argp_program_version= "rainboy'parser ver1.0";
const char *argp_program_bug_address = "rainboylvx@qq.com";
char args_doc[]= "args_doc";
char doc[]= "all doc";

struct argp_option argp_parse_opts[]= {
    //长名,短名,     "arg名",            flag,                 doc,     group
    {"ct",    't',  "MAX_CPU_TIME",       0, "set max_cpu_time(in ms)",     1},
    {"rt",    'T',  "MAX_REAL_TIME",      0, "set max_real_time(in ms)",    2},
    {"ml",    'm',  "MAX_MEMORY",         0, "set max_memory(in byte)",           3},
    {"sl",    's',  "MAX_STACK",          0, "set max_stack(in byte)",            4},
    {"pn",    'p',  "MAX_PROCESS_NUMBER", 0, "set max_process_number",          5},
    {"ol",    'O',  "MAX_OUTPUT_SIZE",    0, "set max_output_size(in byte)",      6},
    {"exe",   'E',  "EXE_PATH",           0, "set exe_path",                    7},
    {"in",    'i',  "INPUT_PATH",         0, "set input_path",                  8},
    {"out",   'o',  "OUTPUT_PATH",        0, "set output_path",                 9},
    {"err",   'e',  "ERROR_PATH",         0, "set error_path",                  10},
    {"arg",   'a',  "ARG1,ARG2,ARG3...",  0, "set args",                        11},
    {"env",   'v',  "ENV1,ENV2,ENV3...",  0, "set env",                         12},
    {"log",   'l',  "LOG_PATH",           0, "set log_path",                    13},
    {"sec",   'S',  "SECCOMP_RULE_NAME",  0, "set seccomp_rule_name",           14},
    {"gid",   'g',  "GID",                0, "set gid",                         15},
    {"uid",   'u',  "UID",                0, "set uid",                         16},
    {"inf",   501,  0,                    0, "print more info of result",       17},
    {"debug", 502,  0,                    0, "show more debug message",         18},
    {0},        //结束
};

error_t arg_parser(int key, char *arg,struct argp_state *state){
    struct config * jconfig = (struct config *)state->input;

    int i = 0;
    int idx = 0;
    int j;
    int len;
    switch(key){
        case 't':
            jconfig->max_cpu_time = atoi(arg);
            args_stat.is_set_ct = 1;
            break;
        case 'T':
            jconfig->max_real_time= atoi(arg);
            args_stat.is_set_rt = 1;
            break;
        case 'm':
            jconfig->max_memory = atoi(arg); //byte
            args_stat.is_set_ml = 1;
            break;
        case 's':
            jconfig->max_stack = atol(arg); //byte
            args_stat.is_set_sl = 1;
            break;
        case 'p':
            jconfig->max_process_number = atoi(arg);
            args_stat.is_set_pn = 1;
            break;
        case 'O':
            jconfig->max_output_size = atol(arg);
            args_stat.is_set_ol=1;
            break;
        case 'E':
            jconfig->exe_path = arg;
            args_stat.is_set_exe = 1;
            break;
        case 'i':
            jconfig->input_path = arg;
            args_stat.is_set_in = 1;
            break;
        case 'o':
            jconfig->output_path = arg;
            args_stat.is_set_out = 1;
            break;
        case 'e':
            jconfig->error_path = arg;
            args_stat.is_set_err = 1;
            break;
        case 'a':
            i = 0;
            idx = 0;
            len = strlen(arg);
            for(j=0;j<len;j++){
                if(arg[j] != ',')
                    _args[i][idx++] = arg[j];
                else{
                    jconfig->args[i+1] = _args[i];
                    i++;
                    idx=0;
                }
            }
            jconfig->args[i+1] = _args[i];
            args_stat.is_set_arg = 1;
            break;
        case 'v':
            i = 0;
            idx = 0;
            len = strlen(arg);
            for(j=0;j<len;j++){
                if(arg[j] != ',')
                    _env[i][idx++] = arg[j];
                else{
                    jconfig->env[i] = _env[i];
                    i++;
                    idx=0;
                }
            }
            jconfig->env[i] = _env[i];
            args_stat.is_set_env = 1;
            break;
        case 'l':
            jconfig->log_path=arg;
            args_stat.is_set_log = 1;
            break;
        case 'S':
            //checker
            if(strlen(arg) == 0){
                jconfig->seccomp_rule_name=NULL;
                args_stat.is_set_sec= 1;
            }
            else if( strcmp(arg,default_seccomp[0])==0 || strcmp(arg,default_seccomp[1]) == 0){
                jconfig->seccomp_rule_name=arg;
                args_stat.is_set_sec= 1;
            }
            else{
                fprintf(stderr,"your args:'sec' is wrong!\n");
                argp_usage(state);
            }
            break;
        case 'g':
            jconfig->gid = atoi(arg);
            args_stat.is_set_gid= 1;
            break;
        case 'u':
            jconfig->uid = atoi(arg);
            args_stat.is_set_uid= 1;
            break;
        case 501:
            args_stat.is_set_moreinfo = 1;
            break;
        case 502:
            args_stat.is_set_debug= 1;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

struct argp _argp_ = {
    argp_parse_opts,
    arg_parser,
    args_doc,
    doc
};

void init_config(){
    memset(&judge_config,0,sizeof(judge_config));
    memset(&args_stat,0,sizeof(args_stat));
}

void parse_args(int argc,char **argv){
    argp_parse(&_argp_,argc,argv,ARGP_NO_ARGS | ARGP_IN_ORDER, 0, &judge_config);

    if(judge_config.max_real_time == 0 )
        judge_config.max_real_time = 2*judge_config.max_cpu_time <=0?-1:2*judge_config.max_cpu_time;

    judge_config.args[0] = judge_config.exe_path;

    //checker must to set
    if(( args_stat.is_set_ct|| args_stat.is_set_rt|| args_stat.is_set_ml|| args_stat.is_set_sl|| args_stat.is_set_pn|| args_stat.is_set_ol|| args_stat.is_set_exe|| args_stat.is_set_in|| args_stat.is_set_out|| args_stat.is_set_err|| args_stat.is_set_arg|| args_stat.is_set_env|| args_stat.is_set_log|| args_stat.is_set_sec|| args_stat.is_set_gid|| args_stat.is_set_uid) == 0) {
        fprintf(stderr,"you must set args,try --help\n");
        exit(1);
    }
}

/* --------------debug -------------------- */
void print_config(){
    fprintf(stderr,"max_cpu_time %d\n", judge_config.max_cpu_time                          );
    fprintf(stderr,"max_real_time %d\n", judge_config.max_real_time                         );
    fprintf(stderr,"max_memory %ld\n", judge_config.max_memory                            );
    fprintf(stderr,"max_stack %ld\n", judge_config.max_stack                             );
    fprintf(stderr,"max_process_number %d\n", judge_config.max_process_number                    );
    fprintf(stderr,"max_output_size %ld\n", judge_config.max_output_size                       );
    fprintf(stderr,"exe_path %s\n", judge_config.exe_path                             );
    fprintf(stderr,"input_path %s\n", judge_config.input_path                           );
    fprintf(stderr,"output_path %s\n", judge_config.output_path                          );
    fprintf(stderr,"error_path %s\n", judge_config.error_path                           );
    int i;

    fprintf(stderr,"__args__:\n");
    for(i=0;i<ARGS_MAX_NUMBER;i++){
        if( judge_config.args[i] != 0)
            fprintf(stderr,"\t%s\n", judge_config.args[i]);
        else
            break;
    }
    fprintf(stderr,"__env__:\n");
    for(i=0;i<ENV_MAX_NUMBER;i++){
        if( judge_config.env[i] != 0)
            fprintf(stderr,"\t%s\n", judge_config.env[i]);
        else
            break;
    }

    fprintf(stderr,"log_path %s\n", judge_config.log_path                             );
    fprintf(stderr,"seccomp_rule_name %s\n", judge_config.seccomp_rule_name                    );
    fprintf(stderr,"uid %d\n", judge_config.uid                                   );
    fprintf(stderr,"gid %d\n", judge_config.gid                                   );
}
/* --------------debug end -------------------- */


int main(int argc,char **argv){
    init_config();
    parse_args(argc,argv);
    if( args_stat.is_set_debug)
        print_config();
    run(&judge_config,&_result);

    //输出result
    if(args_stat.is_set_moreinfo)
    printf("result: %s\ncpu_time(ms): %d\nreal_time(ms): %d\nmemory(byte): %ld\nsignal: %d\nerror: %d\nexit_code: %d\n",
          _result_msg[  _result.result+1],
            _result.cpu_time,
            _result.real_time,
            _result.memory,
            _result.signal,
            _result.error,
            _result.exit_code);
    else
    printf("%d %d %d %ld %d %d %d",_result.result,
            _result.cpu_time,
            _result.real_time,
            _result.memory,
            _result.signal,
            _result.error,
            _result.exit_code);
    return 0;
}
