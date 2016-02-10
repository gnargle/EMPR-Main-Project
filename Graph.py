import pygame #pygame libraries
from pygame.locals import *
import serial #getting data from mbed
import sys #exiting graph
import thread #thread function
import threading #thread function
import math #maths calculations

colour = (255,255,255) #background colour

def start_threads(): #so serial data and graph can run at same time utilise multiple cores maximise concurrency.
        t = threading.Thread(target=draw_tapemeasureplot, args = ())
        t.daemon = True #when finishes it closes
        t.start() #starts thread 
        t2 = threading.Thread(target=get_Serialdata, args = ())
        t2.daemon = True
        t2.start()    

def draw_overheadplot(maxval, minval, value,maximum,minimum): #overhead plot
    pygame.display.set_caption("Overhead Mode") #set title
    x = 50 #reuse x and y later
    y = 50
    while x < 770: #overhead plot
        pygame.draw.line(screen, colour, (x,50),(x,350),1 )
        x+=20
    while y < 370:
        pygame.draw.line(screen, colour, (50,y),(750,y),1 )
        y+=20
    draw_buttons() 
    x = 50 + 200 #or do one with a fixed axis
    y = (get_max(value,maximum) - get_min(value,minimum))
    if y == 0:
        y = 0
    else:        
        y = 300/y
    value = (value - minimum)
    y = (y*value) + 50
    plot_point(x,y)    

def set_calibrationpoints(points,step): #choose what the calibration points are
    for val in points:
        if val % 5 == 0:
            val = val / 2.5 #work math out
            pygame.draw.polygon(screen, (0,255,0), ((45+(val*step), 370), (55+(val*step), 370), (55+(val*step), 360), (60+(val*step), 360), (50+(val*step), 350), (40+(val*step), 360), (45+(val*step), 360)))
        else:
            print "Please enter a number divisble by 5!"
        
def draw_radarplot(): #radar mode
    pygame.display.set_caption("Radar Mode")
    z = 10
    count = 0 #changes size of lines every two
    while z < 400: #radius = 380
        if (count % 2 == 0):  
            pygame.draw.circle(screen, (0,0,255), (400, 400), z, 2)
        else:
            pygame.draw.circle(screen, (0,0,200), (400, 400), z, 1)    
        z += 20
        count += 1
    draw_buttons()
    plotoncircle(400,400,400,225) # 270 == 90, 180 == 0, 360 = 180 need to add 180 to angle    

def draw_tapemeasureplot(distance):  #tape measure mode 
    font = pygame.font.SysFont("Arial", 25) #number font 
    points = [0,5,10,15,20,25,30,35,40,45,50]
    step = 35
    pygame.display.set_caption("Tape Measure Mode")
    if distance <= 0: #draws graph grid to screen
        s = pygame.Surface((0,300))
    elif distance > 700: 
        s = pygame.Surface((700,300))
    else:
        s = pygame.Surface((distance,300))
        lines.append(((50 + distance, 200)))
        if len(lines) >= 2:
            draw_lines(lines)     
    s.fill(100)
    screen.blit(s,(50,50))
    q = 175
    y = 10
    draw_buttons()
    set_calibrationpoints(points,step)
    numbers = font.render(str(0), True, colour)
    screen.blit(numbers, (45,25))
    numbers = font.render(str(5), True, colour)
    screen.blit(numbers, (115,25))
    for x in range(9):
        numbers = font.render(str(y), True, colour)
        screen.blit(numbers, (q,25))
        q += 70
        y += 5
    x = 85
    count = 1
    pygame.draw.rect(screen ,colour, (50,50,700,300), 4) #draws bargraph to screen
    while x < 735:   
        if (count % 2 == 0):
            pygame.draw.line(screen, colour, (x, 50), (x,350),3)
        else:    
            pygame.draw.line(screen, colour, (x, 50), (x,350),1)
        x += 35
        count += 1
    if len(lines) >= 2:
            draw_lines(lines)    #5 10 25 50 need to send data to the mbed board, do boxes and then highlight selected ones    
    
def plotoncircle(xcenter, ycenter, distance, angle): #math for moving around a circle
    angle = math.radians(angle) #convert degrees to radians
    x0 = xcenter #sets center need to translate to
    y0 = ycenter
    x = x0 + (distance * math.cos(angle)) #trig on the point
    y = y0 + (distance * math.sin(angle))
    plot_point(x,y)

def draw_multiviewplot(): #multiview mode
    pygame.display.set_caption("Multiview Mode")
    pygame.draw.line(screen, colour, (400,0), (400,400),3)
    pygame.draw.line(screen, colour, (600,200), (200,200),3)
    pygame.draw.circle(screen, (0,0,255), (400, 200), 200, 2)
    draw_buttons()
    
def get_max(maxval,maximum): #for xmax and ymax
    if maxval > maximum:
        maximum = maxval
        return maxval;
    else:
        return maximum;

def get_min(minval,minimum): #for xmin and ymin
    if minval > minimum:
        minimum = minval
        return minval;
    else:
        return minimum;     

def graph_values(val,minimum,maximum): #gets int then rounds and gets segments
    maxint = int(round(maximum))
    minint = int(round(minimum))
    difference = (maxint - minint)

def draw_buttons(): #buttons onto screen and functionality
    global mode
    buttonfont = pygame.font.SysFont("Arial", 10) #font of buttons
    mouse = pygame.mouse.get_pos() 
    click = pygame.mouse.get_pressed() #check if you have clicked the mouse 
    if mode == "radar": #as radar buttons have a different layout as they were going through the graph
        if 5+40 > mouse[0] > 5 and 10+40 > mouse[1] > 10: #changes colour if mouse is over button
            pygame.draw.rect(screen, (178,102,255),(5,10,40,40)) 
            if click[0] == 1: #if mouse is clicked button does a function
                mode = "tape"
        else:
            pygame.draw.rect(screen, (204,153,255),(5,10,40,40))

        if 5+40 > mouse[0] > 5 and 60+40 > mouse[1] > 60:
            pygame.draw.rect(screen, (0,255,128),(5,60,40,40)) 
            if click[0] == 1:
                mode = "overhead"
        else:
            pygame.draw.rect(screen, (153,255,204),(5,60,40,40))

        if 5+40 > mouse[0] > 5 and 110+40 > mouse[1] > 110:
            pygame.draw.rect(screen, (255,102,102),(5,110,40,40))
            if click[0] == 1:
                mode = "radar"
        else:
            pygame.draw.rect(screen, (255,153,153),(5,110,40,40))

        if 5+40 > mouse[0] > 5 and 160+40 > mouse[1] > 160:
            pygame.draw.rect(screen, (255,153,51),(5,160,40,40))
            if click[0] == 1:
                mode = "multi"
        else:
            pygame.draw.rect(screen, (255,204,153),(5,160,40,40))

        if 55+40 > mouse[0] > 55 and 10+40 > mouse[1] > 10:
            pygame.draw.rect(screen, (51,153,255),(55,10,40,40))
            if click[0] == 1:
                sensor = "ir"
        else:
            pygame.draw.rect(screen, (153,204,255),(55,10,40,40)) 

        if 55+40 > mouse[0] > 55 and 60+40 > mouse[1] > 60:
            pygame.draw.rect(screen, (255,255,0),(55,60,40,40))
            if click[0] == 1:
                sensor = "us"
        else:
            pygame.draw.rect(screen, (255,255,204),(55,60,40,40))

        button = buttonfont.render("IR", True, (0,0,0))
        screen.blit(button, (70,25))
        button = buttonfont.render("US", True, (0,0,0))
        screen.blit(button, (69,75))  
        button = buttonfont.render("TAPE", True, (0,0,0))
        screen.blit(button, (10,25))
        button = buttonfont.render("OVER", True, (0,0,0))
        screen.blit(button, (11,75))
        button = buttonfont.render("RADAR", True, (0,0,0))
        screen.blit(button, (8,125))
        button = buttonfont.render("MULTI", True, (0,0,0))
        screen.blit(button, (10,175))
    
    else:
        mouse = pygame.mouse.get_pos()
        if 5+40 > mouse[0] > 5 and 100+40 > mouse[1] > 100:
            pygame.draw.rect(screen, (178,102,255),(5,100,40,40))
            if click[0] == 1:
                sensor = "us"
        else:
            pygame.draw.rect(screen, (204,153,255),(5,100,40,40))

        if 5+40 > mouse[0] > 5 and 150+40 > mouse[1] > 150:
            pygame.draw.rect(screen, (0,255,128),(5,150,40,40)) 
            if click[0] == 1:
                mode = "tape"
        else:
            pygame.draw.rect(screen, (153,255,204),(5,150,40,40))

        if 5+40 > mouse[0] > 5 and 200+40 > mouse[1] > 200:
            pygame.draw.rect(screen, (255,102,102),(5,200,40,40))
            if click[0] == 1:
                mode = "overhead"
        else:
            pygame.draw.rect(screen, (255,153,153),(5,200,40,40))

        if 5+40 > mouse[0] > 5 and 250+40 > mouse[1] > 250:
            pygame.draw.rect(screen, (255,153,51),(5,250,40,40))
            if click[0] == 1:
                mode = "radar"
        else:
            pygame.draw.rect(screen, (255,204,153),(5,250,40,40))

        if 5+40 > mouse[0] > 5 and 50+40 > mouse[1] > 50:
            pygame.draw.rect(screen, (51,153,255),(5,50,40,40))
            if click[0] == 1:
                sensor = "ir"
        else:
            pygame.draw.rect(screen, (153,204,255),(5,50,40,40)) 

        if 5+40 > mouse[0] > 5 and 300+40 > mouse[1] > 300:
            pygame.draw.rect(screen, (255,255,0),(5,300,40,40))
            if click[0] == 1:
                mode = "multi"
        else:
            pygame.draw.rect(screen, (255,255,204),(5,300,40,40))

        button = buttonfont.render("IR", True, (0,0,0)) #draws buttons to screen
        screen.blit(button, (20,65))
        button = buttonfont.render("US", True, (0,0,0))
        screen.blit(button, (19,115))  
        button = buttonfont.render("TAPE", True, (0,0,0))
        screen.blit(button, (10,165))
        button = buttonfont.render("OVER", True, (0,0,0))
        screen.blit(button, (11,215))
        button = buttonfont.render("RADAR", True, (0,0,0))
        screen.blit(button, (8,265))
        button = buttonfont.render("MULTI", True, (0,0,0))
        screen.blit(button, (10,315))

def plot_point(xcenter,ycenter): #draws cross point
    pygame.draw.line(screen, (0,255,0),(xcenter - 2.5,ycenter - 2.5),(xcenter + 2.5,ycenter + 2.5),1)
    pygame.draw.line(screen, (0,255,0),(xcenter + 2.5,ycenter -2.5),(xcenter - 2.5,ycenter + 2.5),1)

def draw_lines(lines): #draw overheadlines
    pygame.draw.lines(screen, (0,255,0), False, lines,1)
   
def get_Serialdata(): #get data from mbed
    mbed = serial.Serial()
    mbed.port = "/dev/ttyACM0"
    mbed.bytesize = serial.EIGHTBITS
    mbed.parity = serial.PARITY_NONE
    mbed.stopbits = serial.STOPBITS_ONE
    mbed.open()

    while True:
       x = mbed.realine()
       x = x.strip()
       print x     
    mbed.close()  

xcenter = 300 #test plotpoint
ycenter = 300 #test plotpoint
xdistance = 400 #test circleplot
ydistance = 400 #test circleplot
angle = 90 #test circleplot
lines = [] #make lines global for draw
pygame.init() #essential 
sensor = "ir" #initial sensor
mode = "tape" #initial mode 

pygame.mouse.set_visible(1) #enables use of mouse
screen=pygame.display.set_mode((800,400)) #set and lock display size do not change as math is dependant on size
maxval = 100 #test overhead
minval = 0 #test overhead
maximum = 0 #test overhead
minimum = 0 #test overhead
distance =50 #test overhead
value = 0 #test overhead
while True: #main game loop (updates graph status)
    for event in pygame.event.get():
        if event.type == QUIT: #quits if condition is meet (cross in corner pressed)
            pygame.display.quit()
            pygame.quit()
            sys.exit()
    screen.fill((0,0,0))        
    if mode == "tape":   
        #distance = input ("Please enter distance: ")
        draw_tapemeasureplot(distance)  
    elif mode == "overhead":
        #value = input ("Please enter value: ")
        draw_overheadplot(maxval, minval, value,maximum,minimum)
    elif mode == "radar":
        draw_radarplot()
    elif mode == "multi":
        draw_multiviewplot()
            
    pygame.display.update() #update screen needs to be outside loop or will only update on click



