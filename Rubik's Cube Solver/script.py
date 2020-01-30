import numpy as np
import math
import cv2
import webcolors
from time import sleep

webcolorReds = ['brown', 'crimson', 'darkred', 'firebrick', 'indianred', 'lightcoral', 'maroon', 'orangered', 'red',
                'tomato', 'black']
webcolorOranges = ['chocolate', 'coral', 'darkgoldenrod', 'darkorange', 'salmon', 'darksalmon', 'goldenrod', 'lightsalmon', 'orange', 'peru', 'sandrybrown',
                   'sienna', 'saddlebrown']
webcolorYellows = ['gold', 'khaki', 'lemonchiffon', 'lightgoldenrodyellow', 'lightyellow', 'mocassin', 'navajowhite', 'wheat', 'yellow',
                   'palegoldenrod', 'darkkhaki', 'olive']
webcolorGreens = ['chartreuse', 'darkgreen', 'darkolivegreen', 'darkseagreen', 'forestgreen', 'green', 'greenyellow',
                  'lawngreen', 'lightgreen', 'lime', 'limegreen', 'mediumseagreen', 'olivedrab', 'seagreen', 'springgreen',
                  'yellowgreen', 'palegreen']
webcolorBlues = ['aqua', 'aquamarine', 'blue', 'blueviolet', 'cadeblue', 'cornflowerblue', 'cyan', 'darkblue', 'darkcyan',
                 'darkslateblue', 'darkslategray', 'darkslategrey', 'deepskyblue', 'dodgerblue',
                 'indigo', 'lightskyblue', 'mediumblue', 'midnightblue', 'navy', 'royalblue', 'skyblue', 'slateblue',
                 'steelblue', 'slategray']
webcolorWhites = ['aliceblue', 'antiquewhite', 'azure', 'floralwhite', 'gainsboro', 'ghostwhite', 'honeydew', 'ivory', 'lavender',
                  'lightcyan', 'lightgray', 'lightgrey', 'mintcream', 'seashell', 'snow', 'white', 'whitesmoke', 'dimgray', 'gray']



def closest_colour(requested_colour):
    min_colours = {}
    for key, name in webcolors.css3_hex_to_names.items():
        r_c, g_c, b_c = webcolors.hex_to_rgb(key)
        rd = (r_c - requested_colour[0]) ** 2
        gd = (g_c - requested_colour[1]) ** 2
        bd = (b_c - requested_colour[2]) ** 2
        min_colours[(rd + gd + bd)] = name
    return min_colours[min(min_colours.keys())]


def get_colour_name(requested_colour):
    try:
        closest_name = actual_name = webcolors.rgb_to_name(requested_colour)
    except ValueError:
        closest_name = closest_colour(requested_colour)
        actual_name = None
    return actual_name, closest_name


square1 = (240, 160)
square2 = (320, 160)
square3 = (400, 160)
square4 = (240, 240)
square5 = (320, 240)
square6 = (400, 240)
square7 = (240, 320)
square8 = (320, 320)
square9 = (400, 320)
squares = [square1, square2, square3, square4, square5, square6, square7, square8, square9]

radius = 30
img_counter = 0
colorBlue = (255, 0, 0)

key = cv2.waitKey(1)
webcam = cv2.VideoCapture(0)
sleep(2)

while True:
    check, frame = webcam.read()
    frame = cv2.flip(frame, 1)
    cv2.circle(frame, square1, radius, colorBlue, 1)
    cv2.circle(frame, square2, radius, colorBlue, 1)
    cv2.circle(frame, square3, radius, colorBlue, 1)
    cv2.circle(frame, square4, radius, colorBlue, 1)
    cv2.circle(frame, square5, radius, colorBlue, 1)
    cv2.circle(frame, square6, radius, colorBlue, 1)
    cv2.circle(frame, square7, radius, colorBlue, 1)
    cv2.circle(frame, square8, radius, colorBlue, 1)
    cv2.circle(frame, square9, radius, colorBlue, 1)
    key = cv2.waitKey(1)
    if key % 256 == 27:
        print("Escape hit, closing...")
        break
    elif key % 256 == 32:
        height, width = frame.shape[:int(2)]
        height = int(height)
        width = int(width)
        offset = 10
        colors = []
        possibleColors = []
        for square in squares:
            blueValues = []
            redValues = []
            greenValues = []
            for i in range(square[0] - offset, square[0]):
                for j in range(square[1] - offset, square[1]):
                    blueValues.append(frame[j, i, 0])
                    greenValues.append(frame[j, i, 1])
                    redValues.append(frame[j, i, 2])
            meanBlue = np.mean(blueValues)
            meanGreen = np.mean(greenValues)
            meanRed = np.mean(redValues)
            color = (math.floor(meanRed), math.floor(meanGreen), math.floor(meanBlue))
            colorName, closestColorName = get_colour_name(color)
            if colorName is not None:
                possibleColors.append(colorName)
                actualColor = colorName
            else:
                possibleColors.append(closestColorName)
                actualColor = closestColorName
            if actualColor in webcolorBlues:
                colors.append('blue')
            elif actualColor in webcolorGreens:
                colors.append('green')
            elif actualColor in webcolorYellows:
                colors.append('yellow')
            elif actualColor in webcolorReds:
                colors.append('red')
            elif actualColor in webcolorOranges:
                colors.append('orange')
            elif actualColor in webcolorWhites:
                colors.append('white')
            else:
                print(actualColor + ' not found in any array')
        print(colors)
        print(possibleColors)
        while 1:
            None
    cv2.imshow("Capturing", frame)

webcam.release()
cv2.destroyAllWindows()
