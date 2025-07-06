#ifndef __PARSE_H
#define __PARSE_H

#include <stdio.h>
#include <string.h>

#define SIZE_BUFF_CHENK 512

typedef struct{
    uint32_t len;
    char buffer[SIZE_BUFF_CHENK];//128
}CommunicationData;

#endif /* __PARSE_H */