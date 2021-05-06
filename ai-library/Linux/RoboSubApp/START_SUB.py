#!/usr/bin/python3

# Author: Theodor Giles
# Created: 7/14/20
# Last Edited 7/27/20
# Description:
# This program is very basic, basically just so we can mess with a variable or two. will be integrated into gui?
#

from task_io_lib import TaskIO

print(' ===== ROBOSUB v2.0 ===== ')

Mission = TaskIO("mission.txt", False, False)
Mission.switch()
Mission.get_tasks()
Mission.terminate()
print("done")
