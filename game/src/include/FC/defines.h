#pragma once

#define global_variable static
#define local_persist   static
#define local_variable  static
#define global_variable static
#define internal        static

#define len(array)(sizeof(array)/sizeof(array[0]))
