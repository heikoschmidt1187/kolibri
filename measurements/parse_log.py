#!/bin/python
import argparse
import matplotlib.pyplot as plt
import re

parser = argparse.ArgumentParser(description='Parse kolibri logfiles')
parser.add_argument('logfile', type=str, help='The logfile to parse')

def main(args) -> int:
    # read all throttle measurements
    throttles = [re.findall(r'^Throttle: ([0-9\.]+)', line) for line in open(args.logfile, 'r')]
    throttles = [float(x[0]) for x in throttles if len(x) == 1]
    
    gyro = [re.findall(r'Gyro raw: R ([\-0-9\.]+) P ([\-0-9\.]+) Y ([\-0-9\.]+)', line) for line in open(args.logfile, 'r')]

    roll = [float(x[0][0]) for x in gyro if len(x) == 1]
    pitch = [float(x[0][1]) for x in gyro if len(x) == 1]
    yaw = [float(x[0][2]) for x in gyro if len(x) == 1]
    
    fig, axs = plt.subplots(4)
    fig.suptitle(args.logfile)

    axs[0].plot(throttles)
    axs[1].plot(roll)
    axs[2].plot(pitch)
    axs[3].plot(yaw)
    
    plt.show()

    return 0

if __name__ == '__main__':
    args = parser.parse_args()
    exit(main(args))