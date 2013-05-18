AC_INIT([sloth], [0.1], [matias.fontanini@gmail.com], [sloth], [http://slothfuzzer.sourceforge.net])

m4_include([m4/ax_cxx_compile_stdcxx_11.m4])
AC_PROG_CXX()
AC_LANG(C++)

AC_PROG_LEX
if test $LEX == ":" 
then
    AC_MSG_ERROR(***flex is not installed***)
fi

AC_PROG_YACC
if test $YACC == "yacc" 
then
    AC_MSG_ERROR(***bison is not installed***)
fi

AC_CHECK_LIB(crypto, MD5, [], [AC_MSG_ERROR(***libcrypto is not installed***)])


AX_CXX_COMPILE_STDCXX_11(noext)

# Check headers

AC_CHECK_HEADERS([openssl/md5.h])

AC_OUTPUT(Makefile)
