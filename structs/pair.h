#ifndef PAIR_H
#define PAIR_H

#define DEFINE_PAIR(T) \
typedef struct { \
   T fi; \
   T se; \
} Pair_##T; \

DEFINE_PAIR(int)

#endif