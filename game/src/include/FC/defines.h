#pragma once

#define len(array)(sizeof(array)/sizeof(array[0]))

#define zero_array(array)\
    for(int i = 0; i < len(array); i++){\
        array[i] = 0;}
