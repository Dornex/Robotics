def replaceString(chars, newChar):
    s = list(chars)
    s[0] = newChar
    return "".join(s)


def checkExist(chars):
    try:
        ceva = chars[1]
    except:
        return False
    else:
        return True


def updateSol(newSol):
    print("Solution:")
    for i in range(len(newSol)):
        print("Step", (i + 1), "-> Move", newSol[i])
        for j in range(i + 1, len(newSol)):
            if newSol[i][0] == 'D':
                if checkExist(newSol[i]) is False:
                    if newSol[j][0] == 'F':
                        newSol[j] = replaceString(newSol[j], 'L')
                    elif newSol[j][0] == 'L':
                        newSol[j] = replaceString(newSol[j], 'B')
                    elif newSol[j][0] == 'B':
                        newSol[j] = replaceString(newSol[j], 'R')
                    elif newSol[j][0] == 'R':
                        newSol[j] = replaceString(newSol[j], 'F')
                else:
                    if newSol[i][1] == "'":
                        if newSol[j][0] == 'F':
                            newSol[j] = replaceString(newSol[j], 'R')
                        elif newSol[j][0] == 'R':
                            newSol[j] = replaceString(newSol[j], 'B')
                        elif newSol[j][0] == 'B':
                            newSol[j] = replaceString(newSol[j], 'L')
                        elif newSol[j][0] == 'L':
                            newSol[j] = replaceString(newSol[j], 'F')
                    else:
                        if newSol[j][0] == 'F':
                            newSol[j] = replaceString(newSol[j], 'B')
                        elif newSol[j][0] == 'B':
                            newSol[j] = replaceString(newSol[j], 'F')
                        elif newSol[j][0] == 'L':
                            newSol[j] = replaceString(newSol[j], 'R')
                        elif newSol[j][0] == 'R':
                            newSol[j] = replaceString(newSol[j], 'L')
            elif newSol[i][0] == 'U':
                if checkExist(newSol[i]) is False:
                    if newSol[j][0] == 'U':
                        newSol[j] = replaceString(newSol[j], 'D')
                    elif newSol[j][0] == 'D':
                        newSol[j] = replaceString(newSol[j], 'U')
                    elif newSol[j][0] == 'F':
                        newSol[j] = replaceString(newSol[j], 'R')
                    elif newSol[j][0] == 'L':
                        newSol[j] = replaceString(newSol[j], 'B')
                    elif newSol[j][0] == 'B':
                        newSol[j] = replaceString(newSol[j], 'L')
                    elif newSol[j][0] == 'R':
                        newSol[j] = replaceString(newSol[j], 'F')
                else:
                    if newSol[i][1] == "'":
                        if newSol[j][0] == 'U':
                            newSol[j] = replaceString(newSol[j], 'D')
                        elif newSol[j][0] == 'D':
                            newSol[j] = replaceString(newSol[j], 'U')
                        elif newSol[j][0] == 'F':
                            newSol[j] = replaceString(newSol[j], 'L')
                        elif newSol[j][0] == 'R':
                            newSol[j] = replaceString(newSol[j], 'B')
                        elif newSol[j][0] == 'B':
                            newSol[j] = replaceString(newSol[j], 'R')
                        elif newSol[j][0] == 'L':
                            newSol[j] = replaceString(newSol[j], 'F')
                    else:
                        if newSol[j][0] == 'U':
                            newSol[j] = replaceString(newSol[j], 'D')
                        elif newSol[j][0] == 'D':
                            newSol[j] = replaceString(newSol[j], 'U')
                        elif newSol[j][0] == 'F':
                            newSol[j] = replaceString(newSol[j], 'F')
                        elif newSol[j][0] == 'B':
                            newSol[j] = replaceString(newSol[j], 'B')
                        elif newSol[j][0] == 'L':
                            newSol[j] = replaceString(newSol[j], 'R')
                        elif newSol[j][0] == 'R':
                            newSol[j] = replaceString(newSol[j], 'L')
            elif newSol[i][0] == 'F':
                if checkExist(newSol[i]) is False:
                    if newSol[j][0] == 'F':
                        newSol[j] = replaceString(newSol[j], 'D')
                    elif newSol[j][0] == 'B':
                        newSol[j] = replaceString(newSol[j], 'U')
                    elif newSol[j][0] == 'L':
                        newSol[j] = replaceString(newSol[j], 'B')
                    elif newSol[j][0] == 'R':
                        newSol[j] = replaceString(newSol[j], 'F')
                    elif newSol[j][0] == 'U':
                        newSol[j] = replaceString(newSol[j], 'L')
                    elif newSol[j][0] == 'D':
                        newSol[j] = replaceString(newSol[j], 'R')
                else:
                    if newSol[i][1] == "'":
                        if newSol[j][0] == 'F':
                            newSol[j] = replaceString(newSol[j], 'D')
                        elif newSol[j][0] == 'B':
                            newSol[j] = replaceString(newSol[j], 'U')
                        elif newSol[j][0] == 'L':
                            newSol[j] = replaceString(newSol[j], 'F')
                        elif newSol[j][0] == 'R':
                            newSol[j] = replaceString(newSol[j], 'B')
                        elif newSol[j][0] == 'U':
                            newSol[j] = replaceString(newSol[j], 'R')
                        elif newSol[j][0] == 'D':
                            newSol[j] = replaceString(newSol[j], 'L')
                    else:
                        if newSol[j][0] == 'F':
                            newSol[j] = replaceString(newSol[j], 'D')
                        elif newSol[j][0] == 'B':
                            newSol[j] = replaceString(newSol[j], 'U')
                        elif newSol[j][0] == 'L':
                            newSol[j] = replaceString(newSol[j], 'R')
                        elif newSol[j][0] == 'R':
                            newSol[j] = replaceString(newSol[j], 'L')
                        elif newSol[j][0] == 'U':
                            newSol[j] = replaceString(newSol[j], 'B')
                        elif newSol[j][0] == 'D':
                            newSol[j] = replaceString(newSol[j], 'F')
            elif newSol[i][0] == 'B':
                if checkExist(newSol[i]) is False:
                    if newSol[j][0] == 'F':
                        newSol[j] = replaceString(newSol[j], 'U')
                    elif newSol[j][0] == 'B':
                        newSol[j] = replaceString(newSol[j], 'D')
                    elif newSol[j][0] == 'L':
                        newSol[j] = replaceString(newSol[j], 'B')
                    elif newSol[j][0] == 'R':
                        newSol[j] = replaceString(newSol[j], 'F')
                    elif newSol[j][0] == 'U':
                        newSol[j] = replaceString(newSol[j], 'R')
                    elif newSol[j][0] == 'D':
                        newSol[j] = replaceString(newSol[j], 'L')
                else:
                    if newSol[i][1] == "'":
                        if newSol[j][0] == 'F':
                            newSol[j] = replaceString(newSol[j], 'U')
                        elif newSol[j][0] == 'B':
                            newSol[j] = replaceString(newSol[j], 'D')
                        elif newSol[j][0] == 'L':
                            newSol[j] = replaceString(newSol[j], 'F')
                        elif newSol[j][0] == 'R':
                            newSol[j] = replaceString(newSol[j], 'B')
                        elif newSol[j][0] == 'U':
                            newSol[j] = replaceString(newSol[j], 'L')
                        elif newSol[j][0] == 'D':
                            newSol[j] = replaceString(newSol[j], 'R')
                    else:
                        if newSol[j][0] == 'F':
                            newSol[j] = replaceString(newSol[j], 'U')
                        elif newSol[j][0] == 'B':
                            newSol[j] = replaceString(newSol[j], 'D')
                        elif newSol[j][0] == 'L':
                            newSol[j] = replaceString(newSol[j], 'R')
                        elif newSol[j][0] == 'R':
                            newSol[j] = replaceString(newSol[j], 'L')
                        elif newSol[j][0] == 'U':
                            newSol[j] = replaceString(newSol[j], 'F')
                        elif newSol[j][0] == 'D':
                            newSol[j] = replaceString(newSol[j], 'B')
            elif newSol[i][0] == 'L':
                if checkExist(newSol[i]) is False:
                    if newSol[j][0] == 'L':
                        newSol[j] = replaceString(newSol[j], 'D')
                    elif newSol[j][0] == 'R':
                        newSol[j] = replaceString(newSol[j], 'U')
                    elif newSol[j][0] == 'B':
                        newSol[j] = replaceString(newSol[j], 'R')
                    elif newSol[j][0] == 'F':
                        newSol[j] = replaceString(newSol[j], 'L')
                    elif newSol[j][0] == 'U':
                        newSol[j] = replaceString(newSol[j], 'B')
                    elif newSol[j][0] == 'D':
                        newSol[j] = replaceString(newSol[j], 'F')
                else:
                    if newSol[i][1] == "'":
                        if newSol[j][0] == 'F':
                            newSol[j] = replaceString(newSol[j], 'R')
                        elif newSol[j][0] == 'B':
                            newSol[j] = replaceString(newSol[j], 'L')
                        elif newSol[j][0] == 'L':
                            newSol[j] = replaceString(newSol[j], 'D')
                        elif newSol[j][0] == 'R':
                            newSol[j] = replaceString(newSol[j], 'U')
                        elif newSol[j][0] == 'U':
                            newSol[j] = replaceString(newSol[j], 'F')
                        elif newSol[j][0] == 'D':
                            newSol[j] = replaceString(newSol[j], 'B')
                    else:
                        if newSol[j][0] == 'F':
                            newSol[j] = replaceString(newSol[j], 'B')
                        elif newSol[j][0] == 'B':
                            newSol[j] = replaceString(newSol[j], 'F')
                        elif newSol[j][0] == 'L':
                            newSol[j] = replaceString(newSol[j], 'D')
                        elif newSol[j][0] == 'R':
                            newSol[j] = replaceString(newSol[j], 'U')
                        elif newSol[j][0] == 'U':
                            newSol[j] = replaceString(newSol[j], 'R')
                        elif newSol[j][0] == 'D':
                            newSol[j] = replaceString(newSol[j], 'L')
            elif newSol[i][0] == 'R':
                if checkExist(newSol[i]) is False:
                    if newSol[j][0] == 'R':
                        newSol[j] = replaceString(newSol[j], 'D')
                    elif newSol[j][0] == 'L':
                        newSol[j] = replaceString(newSol[j], 'U')
                    elif newSol[j][0] == 'F':
                        newSol[j] = replaceString(newSol[j], 'L')
                    elif newSol[j][0] == 'B':
                        newSol[j] = replaceString(newSol[j], 'R')
                    elif newSol[j][0] == 'U':
                        newSol[j] = replaceString(newSol[j], 'F')
                    elif newSol[j][0] == 'D':
                        newSol[j] = replaceString(newSol[j], 'B')
                else:
                    if newSol[i][1] == "'":
                        if newSol[j][0] == 'F':
                            newSol[j] = replaceString(newSol[j], 'R')
                        elif newSol[j][0] == 'B':
                            newSol[j] = replaceString(newSol[j], 'L')
                        elif newSol[j][0] == 'L':
                            newSol[j] = replaceString(newSol[j], 'U')
                        elif newSol[j][0] == 'R':
                            newSol[j] = replaceString(newSol[j], 'D')
                        elif newSol[j][0] == 'U':
                            newSol[j] = replaceString(newSol[j], 'B')
                        elif newSol[j][0] == 'D':
                            newSol[j] = replaceString(newSol[j], 'F')
                    else:
                        if newSol[j][0] == 'F':
                            newSol[j] = replaceString(newSol[j], 'B')
                        elif newSol[j][0] == 'B':
                            newSol[j] = replaceString(newSol[j], 'F')
                        elif newSol[j][0] == 'L':
                            newSol[j] = replaceString(newSol[j], 'U')
                        elif newSol[j][0] == 'R':
                            newSol[j] = replaceString(newSol[j], 'D')
                        elif newSol[j][0] == 'U':
                            newSol[j] = replaceString(newSol[j], 'L')
                        elif newSol[j][0] == 'D':
                            newSol[j] = replaceString(newSol[j], 'R')
    return newSol
