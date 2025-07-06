#ifndef __PARSE_H
#define __PARSE_H

#include <stdio.h>
#include <string.h>
#include "shearch_component.h"
#include "http.h"

#define SIZE_BUFF_CHENK 512

typedef struct{
    uint32_t len;
    char buffer[SIZE_BUFF_CHENK];//128
}CommunicationData;

void vParseTask(void* pvParameters);

#endif /* __PARSE_H */