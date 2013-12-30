#pragma once
#pragma warning(disable : 4786)

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;
typedef uint32 uint;

#define INLINE __inline
#define ASSERT assert

#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <iterator>
#include <algorithm>
#include <hash_set>
#include <hash_map>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <io.h>

#include <windows.h>
#include <WinSock.h>
#include <process.h>

#include "Common.h"
#include "DataBase.h"


#include "ICoordModule.h"
#include "Stage.h"
#include "Status.h"
#include "Effect.h"
#include "Skill.h"
#include "Item.h"
#include "Quest.h"

#include "GameWorld.h"

#include "Network.h"

#include "GameObject.h"
#include "MapEvent.h"
#include "Player.h"
#include "NewServer.h"
#include "Handler.h"



