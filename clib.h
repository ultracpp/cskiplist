/*
 * C RB Hash Map - Hash Map Implementation in C Language
 * Copyright (c) 2024 Eungsuk Jeon
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __C_LIB_H
#define __C_LIB_H

#define POINTER_TO_INT(p)	((int)(p))
#define INT_TO_POINTER(d)	((void*)(int)(d))

#define POINTER_TO_LONG(p)	((long long)(p))
#define LONG_TO_POINTER(lld)	((void*)(long long)(lld))

#define POINTER_TO_DOUBLE(p)	(POINTER_TO_LONG(p)/10000.0)
#define DOUBLE_TO_POINTER(lf)	(LONG_TO_POINTER((long long)(10000*lf)))

#endif
