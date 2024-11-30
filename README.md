# cpp-backstack

Get function call stack information

## Test on

- Windows
  - MSVC 2017
  - MSVC 2019
  - MSVC 2022

## How To Use

1. ensure that it is compiled in **Debug** or **RelWithDebInfo** environment 
2. put the include file [backstack.h](./include/backstack/backstack.h) in your project
3. call marco `PrintBackStack(MSG)` to get stack information
4. print information on the console or file
