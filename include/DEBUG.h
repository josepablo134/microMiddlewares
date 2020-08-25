#ifndef MIDDLEWARES_DEBUG_H_
#define MIDDLEWARES_DEBUG_H_

#ifdef __cplusplus
    extern "C"
    {
#endif

extern void __error__(void);

#ifdef DEBUG
#define ASSERT(expr) do                                                       \
                     {                                                        \
                         if(!(expr))                                          \
                         {                                                    \
                             __error__(void);                                 \
                         }                                                    \
                     }                                                        \
                     while(0)
#else
#define ASSERT(expr)
#endif

#ifdef __cplusplus
    }
#endif

#endif
