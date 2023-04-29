
# SHELL


### Description
This project consists of three parts: file operations, dynamic libraries, and basic shell. It includes the implementation of two small programs, two coding libraries, and a shell program named "stshell".

### Part A - File Operations


### Part B - Dynamic Libraries



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



