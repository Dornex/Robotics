import serial
import cv2
from time import sleep
from rubik_solver import utils
from updateSol import updateSol
import vision

# Mapping cube moves to strings for Arduino
reverseDict = {"L'": "Lr", "R'": "Rr", "F'": "Fr", "B'": "Br", "U'": "Ur", "D'": "Dr",
               "L": "L", "L2": "L2", "R": "R", "R2": "R2", "F": "F", "F2": "F2",
               "B": "B", "B2": "B2", "U": "U", "U2": "U2", "D": "D", "D2": "D2"}


def clearSerialBuffer():
    if ser.in_waiting:
        print("Garbage in buffer:")
    while ser.in_waiting:
        print(ser.readline())


def waitMove():
    sleep(1)
    print("Waiting for next move signal")
    while True:
        if ser.in_waiting:
            data = ser.readline().decode('utf-8').strip()
            if data == 'next':
                break
        sleep(.5)


def rotateFaces(currentStep):
    print('step', currentStep)
    if currentStep == 1:
        clearSerialBuffer()
        ser.flushInput()
        ser.flushOutput()
    if currentStep == 2:
        ser.write(b"push.")
        waitMove()
    elif currentStep == 3:
        ser.write(b"push.")
        waitMove()
    elif currentStep == 4:
        ser.write(b"push.")
        waitMove()
    elif currentStep == 5:
        ser.write(b"push.")
        waitMove()
        ser.write(b"rotateRight.")
        waitMove()
        ser.write(b"push.")
        waitMove()
    elif currentStep == 6:
        ser.write(b"push.")
        waitMove()
        ser.write(b"push.")
        waitMove()
    elif currentStep == 7:
        ser.write(b"push.")
        waitMove()
        ser.write(b'center.')


def scanCube():
    cube = [None] * 54
    numberOfStepsRemaining = 6
    numberOfFaces = 6
    scanningOrder = [1, 3, 6, 5, 2, 4]

    for step in range(numberOfFaces):
        rotateFaces(7 - numberOfStepsRemaining)
        sleep(.5)
        cubeColors = vision.grab_colors()
        if scanningOrder[step] != 5 and scanningOrder[step] != 4:
            for i in range(3):
                cube[(scanningOrder[step] - 1) * 9 + 6 + i] = cubeColors[0][2 - i]
                cube[(scanningOrder[step] - 1) * 9 + 3 + i] = cubeColors[1][2 - i]
                cube[(scanningOrder[step] - 1) * 9 + i] = cubeColors[2][2 - i]
        elif scanningOrder[step] == 5:
            for i in range(3):
                cube[(scanningOrder[step] - 1) * 9 + 0] = cubeColors[0][0]
                cube[(scanningOrder[step] - 1) * 9 + 1] = cubeColors[0][1]
                cube[(scanningOrder[step] - 1) * 9 + 2] = cubeColors[0][2]
                cube[(scanningOrder[step] - 1) * 9 + 3] = cubeColors[1][0]
                cube[(scanningOrder[step] - 1) * 9 + 4] = cubeColors[1][1]
                cube[(scanningOrder[step] - 1) * 9 + 5] = cubeColors[1][2]
                cube[(scanningOrder[step] - 1) * 9 + 6] = cubeColors[2][0]
                cube[(scanningOrder[step] - 1) * 9 + 7] = cubeColors[2][1]
                cube[(scanningOrder[step] - 1) * 9 + 8] = cubeColors[2][2]
        elif scanningOrder[step] == 4:
            for i in range(3):
                cube[(scanningOrder[step] - 1) * 9 + 0] = cubeColors[0][0]
                cube[(scanningOrder[step] - 1) * 9 + 1] = cubeColors[0][1]
                cube[(scanningOrder[step] - 1) * 9 + 2] = cubeColors[0][2]
                cube[(scanningOrder[step] - 1) * 9 + 3] = cubeColors[1][0]
                cube[(scanningOrder[step] - 1) * 9 + 4] = cubeColors[1][1]
                cube[(scanningOrder[step] - 1) * 9 + 5] = cubeColors[1][2]
                cube[(scanningOrder[step] - 1) * 9 + 6] = cubeColors[2][0]
                cube[(scanningOrder[step] - 1) * 9 + 7] = cubeColors[2][1]
                cube[(scanningOrder[step] - 1) * 9 + 8] = cubeColors[2][2]
        numberOfStepsRemaining -= 1
    print(cube)
    rotateFaces(7)

    cubeInStringForm = []
    for color in cube:
        if color == 'white':
            cubeInStringForm.append('w')
        elif color == 'yellow':
            cubeInStringForm.append('y')
        elif color == 'blue':
            cubeInStringForm.append('b')
        elif color == 'green':
            cubeInStringForm.append('g')
        elif color == 'red':
            cubeInStringForm.append('r')
        elif color == 'orange':
            cubeInStringForm.append('o')
    return "".join(cubeInStringForm)


def solve(cubeInStringForm):
    solution = utils.solve(cubeInStringForm, 'Kociemba')
    newSol = []
    clearSerialBuffer()
    sleep(3)
    for step in solution:
        newSol.append(str(step))
    newSol = updateSol(newSol)

    ind = int(0)
    for move in newSol:
        print("Current move: ", move)
        ser.write(b'%s.' % reverseDict[move].encode('utf-8'))
        sleep(.5)
        while ser.in_waiting == 0:
            pass
        data = ser.readline().decode('utf-8').strip()
        if data == 'done':
            ser.flush()
            ind += 1


if __name__ == '__main__':
    ser = serial.Serial('COM6', 9600, timeout=.1)
    webcam = cv2.VideoCapture(0)
    print("Hello, I'm Eduard's Rubik Solver.")
    print("\n\nPreparing...")
    sleep(2)
    while True:
        print("Python and Arduino in sync")
        print("Place scrambled cube with yellow side facing up and red side facing the arm")
        ready = input("Ready? Yes/No: ")
        if ready.strip().upper() == 'YES':
            print("Scanning cube...")
            print("Press SPACE when colors are aligned correctly")
            stringCube = scanCube()
            print("Cube scanned...")
            print("Solving starting.")
            solve(stringCube)
            print("Cube solved")
            again = input("Want to try again? Yes/No: ")
            if again.strip().upper() == 'YES':
                continue
            else:
                break
    print("Thank you!")
