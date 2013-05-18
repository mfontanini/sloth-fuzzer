/*
 * Copyright (c) 2012, Matias Fontanini
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef WIN32
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <errno.h>
#else
    #include <windows.h>
    #include <set>
#endif // WIN32
#include <tuple>
#include <map>
#include <algorithm>
#include "executer.h"
#include "field.h"

executer::executer(const std::string &cmd)
: cmd_template(cmd)
{
    
}

#ifndef WIN32
int redirect_descriptors() 
{
    int null_fd = open("/dev/null", O_RDWR, 0640);
    dup2(null_fd, STDIN_FILENO);
    dup2(null_fd, STDOUT_FILENO);
    dup2(null_fd, STDERR_FILENO);
    return null_fd;
}

std::string signal_string(int code) 
{
    static std::map<int, std::string> strings = {
        { SIGILL, "Illegal instruction" },
        { SIGSEGV, "Segmentation fault" },
        { SIGSTKFLT, "Stack fault" },
        { SIGBUS, "Bus error" },
        { SIGPIPE, "Broken pipe" }
    };
    auto iter = strings.find(code);
    return (iter == strings.end()) ? "Unknown signal" : iter->second;
}

auto executer::do_execute(const std::string &file) -> exec_status
{
    pid_t pid;
    if((pid = fork()) < 0) {
        throw execution_exception("fork failed");
    }
    if(pid) {
        int status = 0;
        waitpid(pid, &status, 0);
        /* Child was terminated by a signal. */
        if(!WIFEXITED(status) && WIFSIGNALED(status) && WTERMSIG(status) != SIGINT && WTERMSIG(status) != SIGALRM) {
            if(WTERMSIG(status) == SIGUSR2) 
                throw execution_exception("executing application failed");
            else {
                return exec_status::killed_by_signal;
            }
        }
        else
            return exec_status::success;
    }
    else {
        int null_fd = redirect_descriptors();
        // args stuff
        std::string app;
        command_parser::arguments_type args;
        std::tie(app, args) = cmd_template.generate_template(file);
        std::vector<const char *> exec_args(args.size() + 2);
        std::transform(args.begin(), args.end(), exec_args.begin() + 1, std::mem_fn(&std::string::c_str));
        exec_args[0] = app.c_str();
        exec_args.back() = nullptr;
        /* When the process runs for more than 7 seconds, it is killed by SIGALRM. */
        alarm(7);
        /* If execution fails, the child process kills ifself using SIGUSR2.
         * The parent process catches this "exception" and notifies the user. */
        if(execvp(app.c_str(), (char* const*)exec_args.data()) == -1) {
            close(null_fd);
            kill(getpid(), SIGUSR2);
        }
        close(null_fd);
        alarm(0);
        exit(0);
    }
    // should never get here
    return exec_status::failed;
}

#else // WIN32

bool is_fatal_exception(DWORD code) 
{
    static std::set<DWORD> ex_codes = {
        EXCEPTION_ACCESS_VIOLATION,
        EXCEPTION_STACK_OVERFLOW,
        EXCEPTION_ARRAY_BOUNDS_EXCEEDED,
        EXCEPTION_ILLEGAL_INSTRUCTION,
        EXCEPTION_STACK_OVERFLOW
    };
    return ex_codes.count(code);
}

bool wait_for_exception(EXCEPTION_RECORD &ex) 
{
    bool ex_thrown = false;
    DEBUG_EVENT de;
    while(!ex_thrown) {
        if(WaitForDebugEvent(&de, (DWORD)100)) {
            switch(de.dwDebugEventCode) {
                case EXCEPTION_DEBUG_EVENT:
                    if(is_fatal_exception(de.u.Exception.ExceptionRecord.ExceptionCode))
                        ex_thrown = true;
                    break;
                case EXIT_PROCESS_DEBUG_EVENT:
                    return false;
                default:
                    ContinueDebugEvent (de.dwProcessId, de.dwThreadId, DBG_CONTINUE);
                    break;
            }
        }
        else
            ContinueDebugEvent (de.dwProcessId, de.dwThreadId, DBG_CONTINUE);
    }
    /* Exception caught! */
    ex = de.u.Exception.ExceptionRecord;
    return ex_thrown;
}

HANDLE set_startup_info(STARTUPINFOA &info) 
{
    info = {};
    info.cb = sizeof(STARTUPINFOA);
    info.dwFlags = STARTF_USESTDHANDLES;
    HANDLE pout = CreateFile((LPCTSTR)"null", GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    info.hStdInput  = pout;
    info.hStdOutput = pout;
    info.hStdError  = pout;
    return pout;
}

auto executer::do_execute(const std::string &file) -> exec_status
{
    exec_status result = exec_status::success;
    STARTUPINFOA siStartupInfo;
    PROCESS_INFORMATION piProcessInfo{};
    HANDLE handle = set_startup_info(siStartupInfo);
    // args stuff
    std::string app, arguments;
    command_parser::arguments_type args;
    std::tie(app, args) = cmd_template.generate_template(file);
    for(auto it = args.begin(); it != args.end(); it++) {
        arguments += *it;
        if(it + 1 != args.end())
            arguments.push_back(' ');
    }
    
    if(!CreateProcessA(app.c_str(), (LPTSTR)arguments.c_str(), 0, 0, 0, NORMAL_PRIORITY_CLASS | DEBUG_PROCESS, 0, 0, &siStartupInfo, &piProcessInfo)) {
        CloseHandle(handle);
        throw execution_exception("executing application failed: " + GetLastError());
    }
    EXCEPTION_RECORD ex_info;
    if(wait_for_exception(ex_info)) {
        result = exec_status::killed_by_signal;
    }
    CloseHandle(handle);
    return result;
}
#endif
