#pragma once
#include "Particle.h"
#include "softap_http.h"

struct Page
{
    const char* url;
    const char* mime_type;
    const char* data;
};

void Main(const char* url, ResponseCallback* cb, void* cbArg, Reader* body, Writer* result, void* reserved);
