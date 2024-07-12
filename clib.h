#ifndef __C_LIB_H
#define __C_LIB_H

#define POINTER_TO_INT(p)	((int)(p))
#define INT_TO_POINTER(d)	((void*)(int)(d))

#define POINTER_TO_LONG(p)	((long long)(p))
#define LONG_TO_POINTER(lld)	((void*)(long long)(lld))

#define POINTER_TO_DOUBLE(p)	(POINTER_TO_LONG(p)/10000.0)
#define DOUBLE_TO_POINTER(lf)	(LONG_TO_POINTER((long long)(10000*lf)))

#endif
