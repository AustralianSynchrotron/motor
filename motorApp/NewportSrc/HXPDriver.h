/*
FILENAME...   HXPDriver.h
USAGE...      Motor driver support for the Newport Hexapod controller.

*/

#include "asynMotorController.h"
#include "asynMotorAxis.h"

#define MAX_HXP_AXES 6
#define HXP_POLL_TIMEOUT 2.0
#define HXP_MOVE_TIMEOUT 100000.0 // "Forever"

// drvInfo strings for extra parameters that the HXP controller supports
#define HXPMoveCoordSysString                "HXP_MOVE_COORD_SYS"
#define HXPStatusString                      "HXP_STATUS"
#define HXPErrorString                       "HXP_ERROR"
#define HXPErrorDescString                   "HXP_ERROR_DESC"
#define HXPMoveAllString                     "HXP_MOVE_ALL"
#define HXPMoveAllTargetXString              "HXP_MOVE_ALL_TARGET_X"
#define HXPMoveAllTargetYString              "HXP_MOVE_ALL_TARGET_Y"
#define HXPMoveAllTargetZString              "HXP_MOVE_ALL_TARGET_Z"
#define HXPMoveAllTargetUString              "HXP_MOVE_ALL_TARGET_U"
#define HXPMoveAllTargetVString              "HXP_MOVE_ALL_TARGET_V"
#define HXPMoveAllTargetWString              "HXP_MOVE_ALL_TARGET_W"

class HXPAxis : public asynMotorAxis
{
public:
  /* These are the methods we override from the base class */
  HXPAxis(class HXPController *pC, int axis);
  void report(FILE *fp, int level);
  asynStatus move(double position, int relative, double min_velocity, double max_velocity, double acceleration);
  asynStatus home(double min_velocity, double max_velocity, double acceleration, int forwards);
  asynStatus stop(double acceleration);
  asynStatus poll(bool *moving);
  asynStatus setClosedLoop(bool closedLoop);

private:
  HXPController *pC_;          /* Pointer to the asynMotorController to which this axis belongs.
                                      Abbreviated because it is used very frequently */
  int moveSocket_;
  int pollSocket_;
  char axisName_;
  char positionerName_[12];
  char errorDesc_[40];
  char errorDescFull_[1024];
  double encoderPosition_;
  double setpointPosition_;
  int axisStatus_;
  double mres_;
  int moving_;
  
friend class HXPController;
};

class HXPController : public asynMotorController {
public:
  HXPController(const char *portName, const char *HXPPortName, int numAxes, double movingPollPeriod, double idlePollPeriod);

  /* These are the methods that we override from asynMotorDriver */
  asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value); /* needed for implementation of moveAll */
  void report(FILE *fp, int level);
  HXPAxis* getAxis(asynUser *pasynUser);
  HXPAxis* getAxis(int axisNo);

  /* These are the methods that are new to this class */
  int moveAll(HXPAxis* pAxis);

protected:
  #define FIRST_HXP_PARAM HXPMoveCoordSys_
  int HXPMoveCoordSys_;
  int HXPStatus_;
  int HXPError_;
  int HXPErrorDesc_;
  int HXPMoveAll_;
  int HXPMoveAllTargetX_;
  int HXPMoveAllTargetY_;
  int HXPMoveAllTargetZ_;
  int HXPMoveAllTargetU_;
  int HXPMoveAllTargetV_;
  int HXPMoveAllTargetW_;
  #define LAST_HXP_PARAM HXPMoveAllTargetW_

  #define NUM_HXP_PARAMS ((int) (&LAST_HXP_PARAM - &FIRST_HXP_PARAM + 1))

private:
  char *IPAddress_;
  int IPPort_;
  int pollSocket_;
  // only needed for profile moves
  //int moveSocket_;
  char firmwareVersion_[100];
  char *axisNames_;

friend class HXPAxis;
};
