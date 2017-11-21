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
}args_stat;

/* ------------------  argp  ------------------------ */
const char *argp_program_version= "rainboy'parser ver1.0";
const char *argp_program_bug_address = "rainboylvx@qq.com";
char args_doc[]= "args_doc";
char doc[]= "all doc";

struct argp_option argp_parse_opts[]= {
    //长名,短名,     "arg名",            flag,                 doc,     group
    {"ct",    't',  "MAX_CPU_TIME",       0, "set max_cpu_time(in second)",     1},
    {"rt",    'T',  "MAX_REAL_TIME",      0, "set max_real_time(in second)",    2},
    {"ml",    'm',  "MAX_MEMORY",         0, "set max_memory(in mb)",           3},
    {"sl",    's',  "MAX_STACK",          0, "set max_stack(in mb)",            4},
    {"pn",    'p',  "MAX_PROCESS_NUMBER", 0, "set max_process_number",          5},
    {"ol",    'O',  "MAX_OUTPUT_SIZE",    0, "set max_output_size(in mb)",      6},
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
            jconfig->max_cpu_time = atoi(arg) * 1000;
            args_stat.is_set_ct = 1;
            break;
        case 'T':
            jconfig->max_real_time= atoi(arg)*1000;
            args_stat.is_set_rt = 1;
            break;
        case 'm':
            jconfig->max_memory = atoi(arg)*1024*1024; //byte
            args_stat.is_set_ml = 1;
            break;
        case 's':
            jconfig->max_stack = atoi(arg)*1024*1024; //byte
            args_stat.is_set_sl = 1;
            break;
        case 'p':
            jconfig->max_process_number = atoi(arg);
            args_stat.is_set_pn = 1;
            break;
        case 'O':
            jconfig->max_output_size = atoi(arg)*1024*1024;
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
                    jconfig->args[i] = _args[i];
                    i++;
                    idx=0;
                }
            }
            jconfig->args[i] = _args[i];
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

    if(judge_config.max_real_time == 0 ||judge_config.max_real_time == -1)
        judge_config.max_real_time = 2*judge_config.max_cpu_time;
    
    //checker must to set
    if(( args_stat.is_set_ct|| args_stat.is_set_rt|| args_stat.is_set_ml|| args_stat.is_set_sl|| args_stat.is_set_pn|| args_stat.is_set_ol|| args_stat.is_set_exe|| args_stat.is_set_in|| args_stat.is_set_out|| args_stat.is_set_err|| args_stat.is_set_arg|| args_stat.is_set_env|| args_stat.is_set_log|| args_stat.is_set_sec|| args_stat.is_set_gid|| args_stat.is_set_uid) == 0) {
        fprintf(stderr,"you must set args,try --help\n");
        exit(1);
    }
}


int main(int argc,char **argv){
    init_config();
    parse_args(argc,argv);
    run(&judge_config,&_result);
    return 0;
}
