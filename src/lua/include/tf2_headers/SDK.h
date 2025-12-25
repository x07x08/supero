#if !defined(IncSDK_Included)
#define IncSDK_Included

// Rules :
// 0. VMT pointers are always the first in an object, unless there are no virtual functions.
// 1. On Linux, destructors (~) are followed by a pointer. Their position may vary.
// 2. Identical inherited functions must be removed from children.
// 3. VMT and member inheritance are separate. VMTs are separate structures.
// 4. Protected members may be copied into derived classes.
// 5. Inheritance applies to both objects and VMTs.

#include "Interfaces.h"
#include "Classes.h"

#endif
