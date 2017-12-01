import _judger
res = _judger.run(max_cpu_time=1000,
        exe_path="main",
        log_path="log",
        input_path="1.in",
        output_path="out",
        error_path="err",
        debug=True
        )
print(res)
