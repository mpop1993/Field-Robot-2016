paths = 180 * [None] # 180  de grade in fata.  -90,0 stanga = [0,90], iar 0,90 dreapta = [90,179]
CONSECUTIVE_PATHS = 10 # consecutive angles free of obstacles.
lim_x = 70
lim_y = 70

# Global variables
left_speed = 50
right_speed = 100
busola = 360
stare = "All ok"

def decision():
    global left_speed
    global stare
    right = 90
    decision_direction = 0 # 1- left 0-right
    decision_angle = 360 # free path angle
    consecutive = 0
    valid_paths = 180 * [None]  # lista cu gradele spre care o poate lua robotul. -90,0 stanga = [0,90], iar 0,90 dreapta = [90,179]
    badpath = 0
    angle_error = 360
    for i in range(0+CONSECUTIVE_PATHS,180-CONSECUTIVE_PATHS):
        if not paths[i]:
            badpath = 0
            for g in range (0,CONSECUTIVE_PATHS):
                if paths[i+g]:
                    badpath = 1
                    break
                if paths[i-g]:
                    badpath = 1
                    break
            if not badpath:
                if angle_error > abs(i-90):
                    angle_error = abs(i-90)
                    decision_direction = 0 if i > 90 else 1

    if angle_error is not 360:
        stare = str(angle_error) + (" left " if decision_direction else " right ")
    else:
        stare = "No solutions found."
    print stare

    left_speed += 1
    return paths

def test(x,y):
    # Verificam pe o distanta de +-50cm in stanga/dreapta/fata obstacolele, daca
    # gasim un culoar liber viram spre el.
    for angl in range(0,180):
        paths[angl] = 0

    for i in range(0,180):
        right = False
        if i > 90:
            i += 180
            right = True
        if (x[i] is not None) and (y[i] is not None):
            if x[i] < lim_x and x[i] > -lim_x:
                if y[i] > 0 and y[i] < lim_y:
                    #print "Obiect detectat la " + ("dreapta " + str(360-i) if right else "stanga " + str(-i)) + " val x " + str(x[i]) + "val y " + str(y[i])
                    tempang = 90 + ((360 - i) if right else -i)
                    paths[tempang] = 1
    return decision()