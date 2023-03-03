# Lab 3 Test Commands for PSU Shell

A test script was created which can be run and the results written out to a .txt file
```c
// using makefile and running the test
$ make && ./psush -t

// using npm
$ npm run test
```

## Manually Test the Shell

```c
ls -l

cat -n /etc/passwd > JUNK

ls -la -F -t -r -h

ls | wc

ls -l | wc -l

ps -jH | grep defunct
ps -jH | grep defunct
ps -jH | grep defunct

ls -l | wc > JUNK

ls -la -F -h -t -r | cat -n | wc -l -w -c -L

ps -Ao user | grep rchaney | sort | uniq -c | sort -nr | head
ps -jH | grep defunct

/*
OUTPUT:
  _znc
  zadmin
  www-data
  whoopsie
*/
cat /etc/passwd | cut -d ':' -f 1 | sort -r | head -n4
ps -jH | grep defunct

cat -n Makefile | rev | tac | rev | tac | cut -f2-

wc -l < /etc/passwd

wc < /etc/passwd | wc

wc -l < /etc/passwd | wc -c > JUNK

cat < /etc/passwd | sort | cat -n > JUNK

ps -elf | grep rchaney | tr r j | awk '{printf("%s\n",$3);}' > JUNK
```

## Run These From Another Terrminal While Your Shell Is Running

```c
// Do NOT run the following commands from within the psush shell. Run them
// from a regular bash shell prompt while psush is running in
// another terminal.
=======================
$ ps -jH -u $LOGNAME

$ ps -jH | grep defunct

$ ps -Ao user | grep -vs '^root\|^gdm' | sort | uniq -c | sort -nr | head

// Checking for Zombies, Must use ANOTHER terminal to run the following command
$ ps -ef | grep ${LOGNAME} | grep defunct | grep -v grep
```
