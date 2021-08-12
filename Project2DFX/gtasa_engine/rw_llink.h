#ifndef _RWLLLINK
#define _RWLLLINK

typedef struct RwLLLink RwLLLink;
struct RwLLLink
{
    RwLLLink *next;
    RwLLLink *prev;
};

#endif // _RWLLLINK