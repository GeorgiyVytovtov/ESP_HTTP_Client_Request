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

typedef struct
{
    HttpRequest httpRequest;
    char error_msg[128];
    bool is_correct_parse;
} ParseHttpRequest;

ParseHttpRequest parse_message(const char *data, uint16_t size);

void vParseTask(void* pvParameters);

#endif /* __PARSE_H */