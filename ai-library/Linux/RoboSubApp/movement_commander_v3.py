#!python3
# Author: Theodor Giles
# Created: 7/14/20
# Last Edited 8/3/20
# Description:
# This program manages the commands/movement/physical control of the RoboSub
#
import time
import pyfirmata
import math

board = pyfirmata.ArduinoMega('/dev/ttyACM0')

# ROBOSUB


class MovementCommander:

    # initialize everything to supposed starting position
    def __init__(self, usingvision, usingpixhawk):
        self.UsingVision = usingvision
        self.UsingPixHawk = usingpixhawk
        if self.UsingVision:
            from ai_lib import AI
            self.VisionAI = AI("TensorFlow_Graph/Tflite")
            print("MovementCommander is using Vision AI...")
        else:
            print("MovementCommander is not using Vision AI...")

        # setting up board serial port
        print("Communicating with Arduino...")
        # something so the serial buffer doesn't overflow
        self.iter8r = pyfirmata.util.Iterator(board)
        self.iter8r.start()
        print("Communication with Arduino started...")

        if self.UsingPixHawk:
            from pixhawk_data import PixHawk
            self.PixHawk = PixHawk()
            print("MovementCommander is using PixHawk...")
        else:
            print("MovementCommander is not using PixHawk...")

        # possible variable I'll use later for determining how "off path/kilter" we are
        # from a designated target/orientation. basically if it's too high of a #,
        # sirens go off and everybody freaks out and the robot tries extra hard to get
        # back on route
        self.OffKilter = 0

        # normal x,y,z orientation stuff, gonna set to sensor values once I get them
        self.Pitch = 0
        self.Roll = 0
        self.Yaw = 0

        self.WantedPitch = 0
        self.Roll = 0
        self.Yaw = 0

        # L1-4, R1-4 power values to set to the thruster hardpoints
        self.PowerL1 = 0
        self.PowerL2 = 0
        self.PowerL3 = 0
        self.PowerL4 = 0
        self.PowerR1 = 0
        self.PowerR2 = 0
        self.PowerR3 = 0
        self.PowerR4 = 0

        # L1-4, R1-4 thruster hardpoints
        self.ThrusterL1 = ThrusterDriver(2)
        self.ThrusterL2 = ThrusterDriver(3)
        self.ThrusterL3 = ThrusterDriver(4)
        self.ThrusterL4 = ThrusterDriver(5)
        self.ThrusterR1 = ThrusterDriver(6)
        self.ThrusterR2 = ThrusterDriver(7)
        self.ThrusterR3 = ThrusterDriver(8)
        self.ThrusterR4 = ThrusterDriver(9)

        # initialize L1-4, R1-4 thruster values to stay still "braked" during initialization
        self.ThrusterL1.SetSpeed(0)
        self.ThrusterL2.SetSpeed(0)
        self.ThrusterL3.SetSpeed(0)
        self.ThrusterL4.SetSpeed(0)
        self.ThrusterR1.SetSpeed(0)
        self.ThrusterR2.SetSpeed(0)
        self.ThrusterR3.SetSpeed(0)
        self.ThrusterR4.SetSpeed(0)

        # string list of movement commands, because I thought I'd make
        # the index number of each command streamlined with other
        # functions, but it seems a bit detrimental the more I work with
        # it.

        # basic: these commands are just normal up, down, turn, etc.
        self.BASIC_MOVEMENT_COMMANDS = [
            "FORWARD",
            "REVERSE",
            "CLOCKWISETURN",
            "COUNTERCLOCKWISETURN",
            "DIVE",
            "SURFACE"]

        # advanced: these commands are much more complicated, will need to
        # develop pathing and a lot of vision/gyro/position integration
        self.ADVANCED_MOVEMENT_COMMANDS = [
            "LOGSTARTPOINT",
            "RETURNTOSTART",
            "MOVETOTARGET",
            "RAMTARGET",
            "FIRETORPEDO"]

        # name of object to target sent to TF/openCV AI
        self.TO_TARGET = ""

        # possible targets, maybe take from labelmap.txt
        self.POSSIBLE_TARGETS = [
            "red buoy",
            "blue buoy",
            "green buoy",
            "orange buoy",
            "gate"]
        self.TargetList = []
        print("MovementCommander initialized...")

    # checking to see if the sent string is actually a command
    def IsStringACommand(self, searchtarg):
        toreturn = False
        for command in self.POSSIBLE_TARGETS:
            if command == searchtarg:
                toreturn = True
        for command in self.BASIC_MOVEMENT_COMMANDS:
            if command == searchtarg:
                toreturn = True
        for command in self.ADVANCED_MOVEMENT_COMMANDS:
            if command == searchtarg:
                toreturn = True
        return toreturn

    # Concept code, basically for checking if the Sub has already seen the detected object.
    def IsTargetInMemory(self, label, x, y, z):
        NewTarget = [label, x, y, z]
        InMemory = False
        for target in self.TargetList:
            # Determining how far something could be next to the said target,
            DistanceConfidence = math.sqrt(target[4]) * 1.5
            WithinX = abs(NewTarget[1] - target[1]) > DistanceConfidence
            WithinY = abs(NewTarget[2] - target[2]) > DistanceConfidence
            WithinZ = abs(NewTarget[3] - target[3]) > DistanceConfidence
            if (target[0] != NewTarget[0]) and WithinX and WithinY and WithinZ:
                InMemory = True
        return InMemory

    # handles the known targets in the surrounding area
    def SaveTargetToMemory(self, label, x, y, z, area):
        TargetInfo = [label, x, y, z, area]
        self.TargetList.append(TargetInfo)

    # handles checking parsed mission.txt with the movement_commands lists
    def CommandControl(self, commandline):
        running = True

        # simple timer for controlling how long the command goes for. will be removed later on
        InitialTime = time.perf_counter()

        # booleans for telling command handler if it's a basic command or an advanced command
        Basic = False
        Advanced = False
        ToTarget = ""

        # for testing - prints out on rpi terminal when ran
        print("Searching commands...")
        j = 0
        i = 0
        # searching commands from mission.txt line
        for ParsedCommand in commandline.strip().split(', | |. |,'):
            for command in self.BASIC_MOVEMENT_COMMANDS:
                if ParsedCommand == command:
                    print("Running command:", command)
                    Basic = True
                    break
                i += 1
            for command in self.ADVANCED_MOVEMENT_COMMANDS:
                if ParsedCommand == command:
                    print("Running command:", command)
                    Advanced = True
                    break
                j += 1
            for target in self.POSSIBLE_TARGETS:
                if ParsedCommand == target:
                    ToTarget = target
        if Advanced or Basic:
            while running:
                # pixhawk updating
                if self.UsingPixHawk:
                    self.PixHawk.UpdateGyro()
                    self.PixHawk.UpdatePosition()
                if Basic:
                    self.BasicCommand(i)
                    ElapsedTime = time.perf_counter() - InitialTime
                    # testing code, forces all commands to end after 10 seconds
                    if ElapsedTime >= 3:
                        # for testing - prints out on rpi terminal when ran
                        print("End of command.")
                        running = False
                if Advanced:
                    if self.UsingVision:
                        self.AdvancedCommand(j, ToTarget)

    # gonna use targets and
    def AdvancedCommand(self, commandnum, targcommand):
        self.VisionAI.process_image(targcommand)
        TargetData = self.VisionAI.get_target_data()
        Label = targcommand
        TargetX = 0
        TargetY = 0
        TargetZ = 0
        TargetArea = 0
        if TargetData[4]:
            if not self.IsTargetInMemory(Label, TargetX, TargetY, TargetZ):
                self.AdvancedMovement(commandnum, TargetData)
        else:
            pass

    def AdvancedMovement(self, commandnum, targetdata):
        pass

    def BasicCommand(self, commandnum, speed=25):
        print("Command: ", commandnum)
        # 0 = FORWARD
        if commandnum == 0:
            self.PowerR1 = speed
            self.PowerR2 = 0
            self.PowerR3 = 0
            self.PowerR4 = speed
            self.PowerL1 = speed
            self.PowerL2 = 0
            self.PowerL3 = 0
            self.PowerL4 = speed

        # 1 = REVERSE
        if commandnum == 1:
            self.PowerR1 = -speed
            self.PowerR2 = 0
            self.PowerR3 = 0
            self.PowerR4 = -speed
            self.PowerL1 = -speed
            self.PowerL2 = 0
            self.PowerL3 = 0
            self.PowerL4 = -speed

        # 2 = CLOCKWISETURN
        if commandnum == 2:
            self.PowerR1 = -speed
            self.PowerR2 = 0
            self.PowerR3 = 0
            self.PowerR4 = -speed
            self.PowerL1 = speed
            self.PowerL2 = 0
            self.PowerL3 = 0
            self.PowerL4 = speed

        # 3 = COUNTERCLOCKWISETURN
        if commandnum == 3:
            self.PowerR1 = speed
            self.PowerR2 = 0
            self.PowerR3 = 0
            self.PowerR4 = speed
            self.PowerL1 = -speed
            self.PowerL2 = 0
            self.PowerL3 = 0
            self.PowerL4 = -speed

        # 4 = DIVE
        if commandnum == 4:
            self.PowerR1 = 0
            self.PowerR2 = -speed
            self.PowerR3 = -speed
            self.PowerR4 = 0
            self.PowerL1 = 0
            self.PowerL2 = -speed
            self.PowerL3 = -speed
            self.PowerL4 = 0

        # 5 = SURFACE
        if commandnum == 5:
            self.PowerR1 = 0
            self.PowerR2 = speed
            self.PowerR3 = speed
            self.PowerR4 = 0
            self.PowerL1 = 0
            self.PowerL2 = speed
            self.PowerL3 = speed
            self.PowerL4 = 0

        self.ThrusterR1.SetSpeed(self.PowerR1)
        self.ThrusterR2.SetSpeed(self.PowerR2)
        self.ThrusterR3.SetSpeed(self.PowerR3)
        self.ThrusterR4.SetSpeed(self.PowerR4)
        self.ThrusterL1.SetSpeed(self.PowerL1)
        self.ThrusterL2.SetSpeed(self.PowerL2)
        self.ThrusterL3.SetSpeed(self.PowerL3)
        self.ThrusterL4.SetSpeed(self.PowerL4)
        print("ThrusterL1 speed: ", self.ThrusterL1.GetSpeed())

    # ending vehicle connection and AI processing after mission completion/fatal error
    def Terminate(self):
        self.ThrusterR1.SetSpeed(0)
        self.ThrusterR2.SetSpeed(0)
        self.ThrusterR3.SetSpeed(0)
        self.ThrusterR4.SetSpeed(0)
        self.ThrusterL1.SetSpeed(0)
        self.ThrusterL2.SetSpeed(0)
        self.ThrusterL3.SetSpeed(0)
        self.ThrusterL4.SetSpeed(0)
        board.exit()
        if self.UsingPixHawk:
            self.PixHawk.Terminate()
        if self.UsingVision:
            self.VisionAI.terminate()


def MapToT200(x):
    in_min = -100.0
    in_max = 100.0
    out_min = 20
    out_max = 170
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min


# dedicated class to driving a specific thruster
# has own PID, thruster, speed
class ThrusterDriver:
    def __init__(self, pin):
        connectstring = "d:" + str(pin) + ":s"
        self.thruster = board.get_pin(connectstring)
        print("Intiializing Thruster: ", connectstring)
        self.thruster.write(95)
        time.sleep(7)
        self.speed = 95

    # sets speed of thruster
    def SetSpeed(self, speed):  # speed is a value between -100 and 100
        self.speed = MapToT200(speed)
        self.thruster.write(self.speed)

    def SetSpeedPID(self, speed, rollpid, pitchpid, yawpid):
        self.speed = speed + rollpid + pitchpid + yawpid
        self.thruster.write(MapToT200(self.speed))

    # returns speed
    def GetSpeed(self):
        return self.speed
