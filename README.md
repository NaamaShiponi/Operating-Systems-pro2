
# SHELL


### Description
This project consists of three parts: 
> 1. Compare files and file copy in Task A.
> 2. Two codecs "codecA" and "codecB" that can encode and decode messages in Task B.
> 3. A shell program named "stshell".

### Part A - File Operations
This task contains two C programs: compare files and file copy.

#### compare files
The compare_files program takes two files as input and compares their content.
If the content is equal, the program returns 0. If the content is different, the program returns 1.

#### COMPILING THE CODE

To compile the code, open a terminal window and navigate to the directory containing the
source code files. Then, enter the following command:

```bash
$ make
```
This command will compile the code into two executable files, "cmp" and "copy",
which can be run from the command line.

#### RUNNING THE CODE

#### File cmp

To run the "cmp" executable, enter the following command:
```bash
$ ./cmp <file1> <file2> [-v] [-f]
```
The program takes two mandatory arguments: file1 and file2, which are the two files to be compared.

##### The following optional arguments are available:

> * -v: Verbose mode. When this flag is used, the program prints the result of the comparison ("equal" or "distinct") to the standard output.

> * -i: Ignore case. When this flag is used, the program performs a case-insensitive comparison.

  
  
#### File copy

To run the "cmp" executable, enter the following command:
```bash
$ ./copy <src_filename> <dest_filename> [-v] [-f]
```
The program takes two mandatory arguments: src_filename and dest_filename, which are the source file and
the destination file, respectively.

##### The following optional arguments are available:

> * -v: Verbose mode. When this flag is used, the program prints "Success" or "Failed" to the standard output, depending on whether the copy was successful or not.

> * -f: Force mode. When this flag is used, the program overwrites the destination file if it already exists. Otherwise, the program prints an error message and exits.

##### Return codes
The file_copy program returns SUCCESS (0) if the copy was successful, and FAILURE (1) otherwise.
  
```bash
$ export LD_LIBRARY_PATH=.
$ make
```

### Part B - Dynamic Libraries
This code contains two codecs, "codecA" and "codecB", that can encode and decode messages.
The "codecA" codec converts uppercase letters to lowercase and lowercase letters to uppercase,
while the "codecB" codec adds three to the ASCII value of each character in the message.

#### COMPILING THE CODE

To compile the code, open a terminal window and navigate to the directory containing the
source code files. Then, enter the following command:

This command will compile the code into two executable files, "encode" and "decode",
which can be run from the command line.

#### RUNNING THE CODE

To run the "encode" executable, enter the following command:
```bash
$ ./encode <codecA/codecB> <message>
```
Replace <codecA/codecB> with the desired codec (either "codecA" or "codecB") and replace
<message> with the message you want to encode. The executable will decode the message and
print it to the console.

To run the "decode" executable, enter the following command:
```bash
$ ./decode <codecA/codecB> <message>
```
Replace <codecA/codecB> with the codec that was used to encode the message and replace
<message> with the encoded message. The executable will decode the message and print it
to the console.

Note that if the number of arguments passed to the executable is not equal to 3, the program
will print an error message and exit.


### Part C - Basic Shell
In this part, we have implemented a basic shell program named "stshell" (st for students).

#### Features:
> * The shell is able to run CMD tools that exist on the system (by fork + exec + wait).
> * The shell is able to stop a running tool by pressing Ctrl+c, but it does not kill the shell itself (by signal handler).
> * The shell is able to redirect output with ">" and ">>" and allow piping with "|" for at least two following pipes. For example, the command "ls -l | grep aaa | wc" is supported.
> * The shell is able to stop itself by the "exit" command.


#### Running part C:
```bash
$ export LD_LIBRARY_PATH=.
$ make
$ ./stshell
```

#### Examples of commands it can run:
```bash
ls -i 
cd ..
cd <path>
ls | grep copy
ls | grep copy | wc
ls | grep copy | wc > temp.txt
cat temp.txt
ls | grep copy | grep .c >> temp.txt
cat temp.txt
exit
```
![image](https://user-images.githubusercontent.com/43779389/235295614-325098cc-a552-4fd2-b24a-744fa395e34a.png)

```bash
echo aaaa > temp1.txt | cat temp1.txt
./encode codecA aaaa > temp2.txt 
cat temp2.txt
./cmp temp1.txt temp2.txt -v
./cmp temp1.txt temp2.txt -v -i
./decode codecA AAAA > temp2.txt 
cat temp2.txt
./cmp temp1.txt temp2.txt -v -i
```
![image](https://user-images.githubusercontent.com/43779389/235295856-78b43c41-b67d-4563-b7e2-84629521dda5.png)

```bash
./encode codecB njxSfas >> temp3.txt 
cat temp3.txt
./copy temp3.txt temp4.txt -v -f 
cat temp4.txt 
./decode codecB qm{Vidv >> temp3.txt 
cat temp3.txt
```
![image](https://user-images.githubusercontent.com/43779389/235296106-2e94b44f-9491-4cb6-a51b-e2bdb12a11c4.png)



### Conclusion
In conclusion, this project implements three parts: file operations, dynamic libraries, and basic shell. It includes two small programs, two coding libraries, and a shell program named "stshell." 



