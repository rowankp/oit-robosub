#!python3
# Author: Theodor Giles
# Created: 7/15/20
# Last Edited 7/27/20
# Description:
# gets data from the simulated/non-simulated pixhawk for attitude, positioning, and maybe some
# other cool tasks it can do
#

from dronekit import connect, VehicleMode
import time
import math

# sitl is basically a simulation, will remove this and have the legitimate ip once I have the pixhawk
# sitl = dronekit_sitl.start_default()  # (sitl.start)
# connection_string = sitl.connection_string()

YAW = 0
PITCH = 1
ROLL = 2
X = 0
Y = 1
Z = 2
NORTH = 0
EAST = 1
DOWN = 2


def MapToAngle(x):
    in_min = -100.0
    in_max = 100.0
    out_min = 0.0
    out_max = 180.0
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min


class PixHawk:
    Gyro = [0.0, 0.0, 0.0]
    Position = [0.0, 0.0, 0.0]
    Angular_Motions = [0.0, 0.0, 0.0]
    Measures = [0.0, 0.0, 0.0]
    Error = [0.0, 0.0, 0.0]
    Previous_Error = [0.0, 0.0, 0.0]
    Error_Sum = [0.0, 0.0, 0.0]
    Error_Delta = [0.0, 0.0, 0.0]
    Kp = [1.0, 1.0, 1.0]
    Ki = [1.0, 1.0, 1.0]
    Kd = [1.0, 1.0, 1.0]
    Yaw_PID = 0.0
    Yaw_P = 0.0
    Yaw_I = 0.0
    Yaw_D = 0.0
    Pitch_PID = 0.0
    Pitch_P = 0.0
    Pitch_I = 0.0
    Pitch_D = 0.0
    Roll_PID = 0.0
    Roll_P = 0.0
    Roll_I = 0.0
    Roll_D = 0.0
    SubtractYaw = 0.0

    def __init__(self):
        # simulation started, connect vehicle object to fake vehicle ip
        # print("Connecting to vehicle on: %s" % (connection_string,))
        # self.vehicle = connect(connection_string,
        #                        wait_ready=True)
        self.vehicle = connect('/dev/ttyACM1', wait_ready=True, baud=115200)
        self.vehicle.wait_ready(True, timeout=300)
        # - wait_ready flag hold the program until all the parameters are been read (=, not .)
        # read info from vehicle
        self.vehicle.wait_ready('autopilot_version')
        print('Autopilot version: %s' % self.vehicle.version)

        self.vehicle.armed = True
        print('Attempting to arm vehicle...')
        # arm vehicle to see position
        print("Wait 10 for GPS and Gyro initialization. ")
        time.sleep(10)

        # - Read the actual position North, East, and Down
        self.vehicle.add_attribute_listener('position', self.position_callback)  # -- message type, callback function
        self.UpdatePosition()
        self.StartingPosition = self.Position

        # - Read the actual attitude: Roll, Pitch, and Yaw
        self.vehicle.add_attribute_listener('attitude', self.gyro_callback)  # -- message type, callback function
        self.UpdateGyro()
        self.SubtractYaw = self.Gyro[YAW]
        self.StartingGyro = self.Gyro
        time.sleep(4)
        print("Starting gyro: ")
        print(self.Gyro)
        print("Starting position: ")
        print(self.Position)
        time.sleep(4)

    # updating gyro data from the pixhawk
    def gyro_callback(self, attr_name, value, extraval):
        self.UpdateGyro()

    # updating position data from the pixhawk
    def position_callback(self, attr_name, value, extraval):
        self.UpdatePosition()

    # parse gyro object data from pixhawk, turn into dictionary to pass to other programs
    def UpdateGyro(self):
        i = 0
        for CommaParse in str(self.vehicle.attitude).split(','):
            if CommaParse is not None:
                for EqualParse in CommaParse.split('='):
                    try:
                        if i == 1:
                            # "angular motion" is basically just the difference between the
                            # last recorded gyro data and the current gyro data. The algorithm
                            # I looked at for this had an IMU that gave the raw gyro data, but
                            # the pixhawk does not return raw gyro data, so I have to improvise.
                            # basically the same value as self.Error, but I have to set it here
                            # so I can get the new gyro data right after.
                            self.Angular_Motions[PITCH] = self.Angular_Motions[PITCH] * 0.7 + (
                                    self.Gyro[PITCH] - (float(EqualParse) * (180 / math.pi))) * 0.3
                            self.Gyro[PITCH] = float(EqualParse) * (180 / math.pi)
                        if i == 3:
                            self.Angular_Motions[YAW] = self.Angular_Motions[YAW] * 0.7 + (
                                    self.Gyro[YAW] - ((float(EqualParse) - self.SubtractYaw) * (180 / math.pi))) * 0.3
                            self.Gyro[YAW] = (float(EqualParse) * (180 / math.pi)) - self.SubtractYaw
                        if i == 5:
                            self.Angular_Motions[ROLL] = self.Angular_Motions[ROLL] * 0.7 + (
                                    self.Gyro[ROLL] - (float(EqualParse) * (180 / math.pi))) * 0.3
                            self.Gyro[ROLL] = float(EqualParse) * (180 / math.pi)
                        i += 1
                    except:
                        pass
                        # print("error reading gyro")
        self.Measures[ROLL] = self.Measures[ROLL] * .5 + self.Gyro[ROLL] * .5
        self.CalculateError()
        self.PID()

    # parse position object data from pixhawk, turn into dictionary to pass to other programs
    def UpdatePosition(self):
        i = 0
        for CommaParse in str(self.vehicle.location.local_frame).split(','):
            if CommaParse is not None:
                for EqualParse in CommaParse.split('='):
                    try:
                        if i == 1:
                            self.Position[NORTH] = float(EqualParse)
                        if i == 3:
                            self.Position[EAST] = float(EqualParse)
                        if i == 5:
                            self.Position[DOWN] = float(EqualParse)
                        i += 1
                    except:
                        pass
                        #print("error reading position")
        # print(self.Position)

    # position read when starting the robosub
    def getStartingPosition(self):
        return self.StartingPosition

    # current position read
    def getPosition(self):
        return self.Position

    def getNorth(self):
        return self.Position[NORTH]

    def getEast(self):
        return self.Position[EAST]

    def getDown(self):
        return self.Position[DOWN]

    # gyro read when starting the robosub
    def getStartingGyro(self):
        return self.StartingGyro

    # current gyro read
    def getGyro(self):
        return self.Gyro

    def getPitch(self):
        return self.Gyro[PITCH]

    def getRoll(self):
        return self.Gyro[ROLL]

    def getYaw(self):
        return self.Gyro[YAW]

    # req for PID calculation
    def CalculateError(self):

        self.Previous_Error[YAW] = self.Error[YAW]
        self.Previous_Error[PITCH] = self.Error[PITCH]
        self.Previous_Error[ROLL] = self.Error[ROLL]

        # error for proportional control
        self.Error[YAW] = self.Angular_Motions[YAW]
        self.Error[PITCH] = self.Angular_Motions[PITCH]
        self.Error[ROLL] = self.Angular_Motions[ROLL]

        # sum of error for integral
        self.Error_Sum[YAW] = self.Error_Sum[YAW] + self.Error[YAW]
        self.Error_Sum[PITCH] = self.Error_Sum[PITCH] + self.Error[PITCH]
        self.Error_Sum[ROLL] = self.Error_Sum[ROLL] + self.Error[ROLL]

        # math for change in error to do derivative
        self.Error_Delta[YAW] = self.Error[YAW] - self.Previous_Error[YAW]
        self.Error_Delta[PITCH] = self.Error[PITCH] - self.Previous_Error[PITCH]
        self.Error_Delta[ROLL] = self.Error[ROLL] - self.Previous_Error[ROLL]

    # pid calculation
    def PID(self):
        # Yaw PID variable setting
        self.Yaw_P = (self.Error[YAW] * self.Kp[YAW])
        self.Yaw_I = (self.Error_Sum[YAW] * self.Ki[YAW])
        self.Yaw_D = (self.Error_Delta[YAW] * self.Kd[YAW])
        self.Yaw_PID = self.Yaw_P + self.Yaw_I + self.Yaw_D

        # Pitch PID variable setting
        self.Pitch_P = (self.Error[PITCH] * self.Kp[PITCH])
        self.Pitch_I = (self.Error_Sum[PITCH] * self.Ki[PITCH])
        self.Pitch_D = (self.Error_Delta[PITCH] * self.Kd[PITCH])
        self.Pitch_PID = self.Pitch_P + self.Pitch_I + self.Pitch_D

        # Roll PID variable setting
        self.Roll_P = (self.Error[ROLL] * self.Kp[ROLL])
        self.Roll_I = (self.Error_Sum[ROLL] * self.Ki[ROLL])
        self.Roll_D = (self.Error_Delta[ROLL] * self.Kd[ROLL])
        self.Roll_PID = self.Roll_P + self.Roll_I + self.Roll_D

    def getYawPID(self):
        return self.Yaw_PID

    def getPitchPID(self):
        return self.Pitch_PID

    def getRollPID(self):
        return self.Roll_PID

    # end command/vehicle running
    def Terminate(self):
        self.vehicle.close()
