sloth
=====

sloth is a smart file fuzzer.

## Why is it a smart fuzzer? ##

The files generated when fuzzing are not just random data. You must 
provide a template file, which describes the structure of the generated 
files.

## How is it different from other smart fuzzers? ##

Every smart fuzzer I've found uses either XML or some programming 
language script to describe the structure of the fuzzed files. Using 
XML is tedious, you must provide starting tags, ending tags, named 
attributes, etc. Using actual programming language scripts forces the 
user to learn both the structure definition directives and the 
programming language itself.

*sloth* uses its own syntax to represent the file structure. For example, 
this template file provides the structure for a basic jpeg file:

<pre><code>templates { 
    segment {
        # the first byte will be 0xff
        block<1> = 255;
        block<1>;
        block<2> = size(data);
        var_block<0, 2000> data;
    };
};
 
str_block = "\xff\xd8\xff\xe0";
block<2> = 16;
str_block = "JFIF\x00\x01\x02";
block<8> = 0; # units
template<segment, 0, 200>; # 0 to 200 segments
</code></pre>

In order to use sloth, you just need to understand a few primitives 
which are used to define blocks.

## Compilation ##

*sloth* requires the following libraries/applications/features:

* libcrypto
* flex
* bison
* A C++ compiler that has nice support for C++11 features(gcc >= 4.7 for example)

In order to compile, just do the usual:

```Shell
./configure
make
```

## Running sloth ##

In order to start test some application, just execute sloth indicating 
the template file to use:

```Shell
# just as an example
./sloth -t /tmp/some_template_file some_application some_app_param1
# fuzz mpg123, for example
./sloth -t templates/mp3 mpg123
# you can also use parameters, "-2" here is a mpg123 parameter
./sloth -t templates/mp3 mpg123 -2
# same as above, {%} indicates the place in which the name of the file
# will be replaced. By default it's replaced at the end of the command
./sloth -t templates/mp3 mpg123 -2 {%}
```

If at any point, the application being tested crashes, the file that
made it crash will be moved to /tmp/fuzzer-NUMBER, where NUMBER will 
be 0 initially, and incremented each time a file makes the application 
crash. If you want to provide a different base name 
(here you'd replace the /tmp/fuzzer-), provide the -d parameter:
 
```Shell
./sloth -t something -d /home/something/persistent_dir/crash-file application
```

Some applications might require the name of the file to have a specific
extension. By default, the generated file will be named /tmp/sloth-file. 
If you want to use another name, use the -o parameter:

```Shell
./sloth -t something -o /dev/shm/sloth.jpg convert {%} /dev/shm/sloth.png
```

## Template file definition ##

sloth uses its own syntax to allow users to represent template files 
that will be used when fuzzing. While fuzzing, every generated file 
will be constructed using the structure provided in the template file. 

The templates' layout is basically a list of blocks. There are different 
kind of blocks, each of them provides different features:

### block ###

blocks can be used to represent fields of static size. When you declare 
a block, you need to provide the size, and optionally a value. If you 
don't provide any value, its contents shall be randomized:

<pre><code>
# defines a 4-bytes block, the contents will be random
block<4>; 
# using a specific value
block<2> = 15;
block<4> = 0xdeadbeef;
# give it a name, and use some function over it
block<50> md5_input;
# will contain the MD5 hash of the contents of md5_input
block<16> = md5(md5_input); 
# expressions are allowed as well
block<1> foo;
block<4> = foo * 15 + 1;
# you use a string as well
block<7> = "HELLO\xde\xad";
</code></pre>

### var_block ###

var_blocks contain a variable amount of data. The minimum and maximum 
amount of data must be provided on their definition:

<pre><code>
# will contain between 0 and 5 bytes
var_block<0, 5>;
 
# 5 to 10 bytes filled with 0
var_block<5, 10> foo = 0;
 
# use the size of the field
block<4> = size(foo);
</code></pre>

### str_block ###

str_blocks are just blocks that must be initialized with a string value. 
The length of the block will be equal to the length of the string:

<pre><code>
# will be a 4 byte block
str_block = "beef";
# 6 bytes
str_block = "hi\xff\x08ho";
</code></pre>

### multi_bit and bitfield ###

multi_bit blocks are just wrappers over several bitfields. bitfields 
represent fields which contain bits instead of bytes. bitfields can be 
assigned a value or expression:

<pre><code>
# wrapper over the inner bitfields
multi_bit {
    bitfield<2> = 0;
    bitfield<4> = 0xff;
    # randomized
    bitfield<2>; 
};
 
# named as well
multi_bit something {
    bitfield<24>;
    bitfield<8> = 0xfe;
};
 
block<16> = md5(something);
</code></pre>

### multi_block ###

multi_blocks are very similar to multi_bit fields. The main difference
is that they hold regular fields, rather than only bitfields.

<pre><code>
# some fields
multi_block my_big_block {
    block<4>;
    block<1> = 0;
    multi_bit {
        bitfield<4>;
        bitfield<4> = 5;
    };
};
</code></pre>


### templates ###

template blocks are very useful when you need to repetedly use a certain 
structure. templates are defined at the beginning of the file, and can 
be instantiated at any point in them:

<pre><code>
# our file begins with this definitions
templates {
    # foo is a template
    foo {
        block<4> bar;
        block<16> = md5(bar);
    };
};
 
# instantiate our template
template<foo>;
 
# instantiate 5 foos
template<foo, 5>;
 
# instantiate between 6 and 8 foos
template<foo, 6, 8> bleh;
 
# will hold 6, 7 or 8 depending on how many fields does bleh contain
block<1> = count(bleh);
</code></pre>

### choice ###

choices are fields that contain several fields, but only one of them 
will be used in each file generation:

<pre><code>
# can contain either 2 or 4 random bytes
choice {
    block<4>;
    block<2>;
};
</code></pre>

### functions ###

There are some functions defined which allow you to initialize your 
field values with some value that depends on other fields' values:

<pre><code>
# some random block
block<50> input;
# crc
block<4> = crc32(input);
# md5
block<16> = md5(input);
# sha1
block<20> = sha1(input);
# always 50
block<2> = size(input);
</code></pre>
