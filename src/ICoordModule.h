typedef char int8;
typedef unsigned char uint8;
typedef unsigned char byte;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;
typedef uint32 uint;

struct VELOCITY_STATUS
{
	float fSpeedX;
	float fSpeedY;
	float fRopeSpeedY;
	float fRopeSpeedX;
	float fDropSpeed;
	float fMaxDropSpeed;
};

class ICoordPhysics
{
public:
	virtual void Process(float fElapsedTime) = 0;
	virtual uint64 GetCurAction() = 0;
	virtual void SetAction(uint64 uiAction) = 0;
	virtual int GetCurSeqIndex() = 0;
	virtual void SetSeqIndex(int iSeqIndex) = 0;
	virtual float GetPosX() = 0;
	virtual float GetPosY() = 0;
	virtual void SetPosX(float fPosX) = 0;
	virtual void SetPosY(float fPosY) = 0;
	virtual void SetDirection(int iDirection) = 0;
	virtual int GetDirection() = 0;
	virtual void SetActivityVelocity(float fDelayTime, float fTime, float fVelocityX, float fVelocityY) = 0;
	virtual void SetActionTime(float fActionTime) = 0;
	virtual void RaiseMapChange() = 0;
	virtual void GetVelocityStatus(VELOCITY_STATUS& VelocityStatus) = 0;
	virtual void SetVelocityStatus(VELOCITY_STATUS& VelocityStatus) = 0;
	virtual void GetLTPosition(byte Buffer[50]) = 0;
};

class ICoordPhysicsSink
{
public:
	virtual void OnActionCommand(uint64 uiAction) = 0;
	virtual void OnActionCompleteCommand(uint64 uiAction) = 0;
	virtual char GetAttribute(int iLayer, int iX, int iY) = 0;
	virtual void GetVelocityRatio(VELOCITY_STATUS& VelocityRatio) = 0;
	virtual void GetMapGroupSize(int& iMapWidth, int& iMapHeight) = 0;
};

class ICoordModule
{
public:
	virtual ICoordPhysics* CreateCoord(ICoordPhysicsSink *pkSink, int iType) = 0;
	virtual void DeleteCoord(ICoordPhysics* pkCoord) = 0;
};

