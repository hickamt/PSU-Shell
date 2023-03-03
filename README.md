## Basic Shell Implementation

This program includes the basic implementation you would expect in a shell such as piping and redirects. Other basic commands built into this shell include:

- quit: exit the shell
- clear: clear the console display
- cd `<directoryName>`
- cwd: get current working directory
- ls: list the cwd
- history: just the last 10 commands entered as menu not using arrow keys
- echo: you know ... you know
- grep and other execvp() commands

A few extra additions I made include the custom shell which was originally static. After testing 'cd' without the feedback expected from the given shell I modified it to include the current working directory path and then added a color scheme. The point reductions received on modifying the SH.. out of this program is SOOO worth it.

## Doing It Different | Run Using Yarn or NPM

UNCONVENTIONAL: As much as I enjoyed learning to build my own make files, I don't like to stop tyring new things once an interesting project is over. I enjoy sovling problems using unconventional methods and feel that compiling and running a C program using package.json is just too fun not to try.

[![Open in StackBlitz](https://developer.stackblitz.com/img/open_in_stackblitz.svg)](https://stackblitz.com/github/hickamt/PSU-Shell/)

## Run From StackBlitz Command Line OR Locally

Of course, you will need NodeJS installed to run using the node package manager (npm).

- [NodeJS (npm) installation instructions](https://nodejs.org/en/download/)

Using NPM

```c
// Build and run the program
$ npm run dev

// Build and display the menu
$ npm run menu

// Build and test the program
$ npm run test

// Clean the directory
$ npm run clean
```

And, running yarn requires that you have yarn installed.

- [yarn installation instructions](https://classic.yarnpkg.com/lang/en/docs/install/#windows-stable)

Using Yarn

```c
// Build and run the program
$ yarn dev

// Build and test the program
$ yarn test

// Clean the directory
$ yarn clean

// Build and display the menu
$ yarn menu
```

## Method of Creation

Building the shell was a group think approach to solving each problem. A few students created a Discord and as I worked through each problem I created notes that other students could use. These notes sparked communication and questions that allowed us to find the best solutions possible within the time frame given.

[Lab 2 Notes](./LAB2_NOTES.md)

This project was in two parts and the notes above were my contribution to help others get started. Though we worked through the theory of implementation as a group, I can state that the code is mine (for better or worse).

## Run the Program with Make

The outfile name is a bit strange, but it stands for

- Portland State University Shell (psush)

You will need gcc installed to run the make command.

- [gcc GNU installation instructions](https://gcc.gnu.org/install/)

```c
// compile and run the outfile
$ make && ./psush

// clean directory
$ make clean
```

## Menu Options

```c
// See the list of options
$ ./psush -h

// Run the automated tests
$ ./psush -t
```
