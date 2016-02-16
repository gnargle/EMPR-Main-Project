import pygame #pygame libraries
from pygame.locals import *
import serial #getting data from mbed
import sys,os #exiting graph
import math #maths calculations
from threading import *

##############################################
#                    MAIN                    #
##############################################
'''def start_threading():
    t1 = Thread(target=Serialdata, args=())
    #t1.daemon = True
    t1.start()'''

def Serialdata(mbed):
	global ir_raw, us_raw, ir, us, max_rot, min_rot, cal_point, sweep_num, line,count
	'''if os.path.exists("data.tmp") == True:
        os.remove("data.tmp")
    mbed = serial.Serial()
    mbed.port = "/dev/ttyACM0"
    mbed.bytesize = serial.EIGHTBITS
    mbed.parity = serial.PARITY_NONE
    mbed.stopbits = serial.STOPBITS_ONE
    #t2 = Thread(target=main, args=())
    #t2.daemon = True
    #t2.start()'''
    #while(1):
	if count % 2 == 0:
		mbed.open()
		x = mbed.readline()
		x = x.strip()
		vals = x.split(';', 10)
		print vals
		ir_raw = int(vals[1])
		us_raw = int(vals[2])
		ir = int(vals[3])
		us = int(vals[4])
		angle = int(vals[5])
		max_rot = int(vals[6])
		min_rot = int(vals[7])
		cal_point = int(vals[8])
		sweep_num = int(vals[9])
		nums = vals[1:10]
		numbers = str(line) + ":"
		for a in range (9):
			if a == 8:
				numbers += nums[a]
			else:
				numbers += nums[a] + ":"
		f = open('data.tmp', 'a')
		lst = [numbers, "\n"]
		f.writelines(lst)
		f.close()
		mbed.close()
		line += 1	
	else:
		pass
	count += 1

def main():
	global mode
	if os.path.exists("data.tmp") == True:
		os.remove("data.tmp")
	mbed = serial.Serial()
	mbed.port = "/dev/ttyACM0"
	mbed.bytesize = serial.EIGHTBITS
	mbed.parity = serial.PARITY_NONE
	mbed.stopbits = serial.STOPBITS_ONE
	done = False
	while not done:
		Serialdata(mbed)
		for event in pygame.event.get():
			if event.type == QUIT:
				done = True
				pygame.quit()
		screen.fill((0,0,0))      
		if mode == "tape":  
			draw_tapemeasureplot()  
		elif mode == "overhead":
			draw_overheadplot()
		elif mode == "radar":
			draw_radarplot()
		elif mode == "multi":
			draw_multiviewplot()   
		pygame.display.update() #update screen needs to be outside loop or will only update on click        

##############################################
#                TAPEMEASURE                 #
##############################################
def draw_tapemeasureplot():  #tape measure mode 
    global font,us_raw, ir_raw, ir, us, screen, cal_point
    if sensor == "ir":
        distance = ir
    elif sensor == "us":
        distance = us    
    point = cal_point
    step = 7
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
    q = 110
    y = 10
    draw_buttons()
    set_calibrationpoints(point,step)
    numbers = font.render(str(0), True, colour)
    screen.blit(numbers, (45,25))
    numbers = font.render(str(100), True, colour)
    screen.blit(numbers, (725,25))

    for x in range(9):
        numbers = font.render(str(y), True, colour)
        screen.blit(numbers, (q-3,25))
        q += 70
        y += 10
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
    '''if len(lines) >= 2:
            draw_lines(lines)'''  

def set_calibrationpoints(points,step): #choose what the calibration points are
            pygame.draw.polygon(screen, (0,255,0), ((45+(points*step), 370), (55+(points*step), 370), (55+(points*step), 360), (60+(points*step), 360), (50+(points*step), 350), (40+(points*step), 360), (45+(points*step), 360))) 

##############################################
#                  OVERHEAD                  #
##############################################
def draw_overheadplot(): #overhead plot
    global us, ir,ir_raw,us_raw, a, sensor, screen, pointlist # need to set points list to zero after mode changes
    pygame.display.set_caption("Overhead Mode") #set title
    z = 0
    if sensor == "us":
        z = us * 2
    elif sensor == "ir":
        z = ir * 2

    if a >= 700: #catches out of bounds
        a = 700
        if z < 0:
            z = 0
        elif z > 350:
            z = 300  
    elif a <= 0:
        a = 0
        if z < 0:
            z = 0
        elif z > 350:
            z = 300          
    elif z < 0:
        z = 0
        if a >= 700:
            a = 700
        elif a <= 0:
            a = 0    
    elif z > 350:
        z = 300
        if a >= 700:
            a = 700
        elif a <= 0:
            a = 0  
    if len(pointlist) >= 35:
        pointlist.pop(0) #if list is full on graph it shifts it along 1
        pointlist.append([(a+50),((-1*z)+350)])
        screen.fill([0,0,0]) #clear point list
        for val in pointlist:
            val[0] -=20
    else:
        screen.fill([0,0,0])
        pointlist.append([(a+50),((-1*z)+350)])
    for val in pointlist:        
        plot_point(val[0],val[1])
    a += 20
    if len(pointlist) == 1:
        pass
    else:  
        draw_lines(pointlist)

    x = 50
    y = 50
    while x < 770: #overhead plot
        pygame.draw.line(screen, (0,51,102), (x,50),(x,350),1 )
        x+=20
    while y < 370:
        pygame.draw.line(screen, (0,51,102), (50,y),(750,y),1 )
        y+=20
    draw_buttons()
##############################################
#                   RADAR                    #
##############################################
def draw_radarplot(): #radar mode
    global angle, max_rot, min_rot, us, ir, us_raw, ir_raw, sensor, mode
    pygame.display.set_caption("Radar Mode")
    radarangle = angle + 180
    if sensor == "ir":
        distance = ir * 2
    elif sensor == "us":
        distance = us * 2    
    z = 10
    pygame.draw.line(screen, (0,51,102), (400, 400), (400, 30), 2)
    pygame.draw.line(screen, (0,51,102), (30, 400), (770, 400), 8)
    pygame.draw.line(screen, (0,51,102), (400, 400), (400 - (370*math.cos(math.radians(45))),400 - (370*math.sin(math.radians(45)))), 2)
    pygame.draw.line(screen, (0,51,102), (400, 400), (400 - (370*math.cos(math.radians(135))),400 - (370*math.sin(math.radians(135)))),2)
    count = 0 #changes size of lines every two
    while z < 380: #radius = 380
        if (count % 2 == 0):  
            pygame.draw.circle(screen, (0,128,255), (400, 400), z, 2)
        else:
            pygame.draw.circle(screen, (0,51,102), (400, 400), z, 1)    
        z += 20
        count += 1
    draw_buttons()
    if distance >= 370: #catches out of bounds
        distance = 370
        if radarangle <= 180:
            radarangle = 0
        elif radarangle >= 360:
            radarangle = 180  
    elif distance <= 0:
        distance = 0
        if radarangle <= 180:
            radarangle = 0
        elif radarangle >= 360:
            radarangle = 180  
    elif radarangle <= 180:
        radarangle = 0
    elif radarangle >= 360:
        radarangle = 180        

    plotoncircle(400,400,distance,radarangle) # 270 == 90, 180 == 0, 360 = 180 need to add 180 to angle    

def plotoncircle(xcenter, ycenter, distance, angle): #math for moving around a circle
    angle = math.radians(angle) #convert degrees to radians
    x0 = xcenter #sets center need to translate to
    y0 = ycenter
    x = x0 + (distance * math.cos(angle)) #trig on the point
    y = y0 + (distance * math.sin(angle))
    plot_point(x,y)
            
##############################################
#                 MULTIVIEW                  #
##############################################
def draw_multiviewplot(): #multiview mode
    global mode, sensor, ir, us, us_raw, ir_raw, max_rot, min_rot, sweep_num, angle
    if sensor == "ir":
        distance = ir
    elif sensor == "us":
        distance = us
    multiangle = angle     
    pygame.display.set_caption("Multiview Mode")
    pygame.draw.line(screen, (0,51,102), (400,40), (400,360),3)
    pygame.draw.line(screen, (0,51,102), (560,200), (240,200),3)
    pygame.draw.circle(screen, (0,128,255), (400, 200), 150, 2)
    pygame.draw.circle(screen, (0,128,255), (400, 200), 125, 1)
    pygame.draw.circle(screen, (0,128,255), (400, 200), 100, 2)
    pygame.draw.circle(screen, (0,128,255), (400, 200), 75, 1)
    pygame.draw.circle(screen, (0,128,255), (400, 200), 50, 2)
    pygame.draw.circle(screen, (0,128,255), (400, 200), 25, 1)

    if sweep_num == 0: #180 -> 0 #need to do angle bounds
        if distance >= 150:
            distance = 150
        elif distance <= 0:
            distance = 0    
        multiangle = abs(multiangle - 180)
        multiangle = math.radians(multiangle)
        x = 400 + (distance * math.cos(multiangle)) #trig on the point
        y = 200 - (distance * math.sin(multiangle))
        plot_point(x,y)             
    
    elif sweep_num == 1: #270 -> 90
        if distance >= 150:
            distance = 150
        elif distance <= 0:
            distance = 0   
        multiangle = abs(multiangle - 270)
        multiangle = math.radians(multiangle)
        x = 400 - (distance * math.cos(multiangle)) #trig on the point
        y = 200 + (distance * math.sin(multiangle))
        plot_point(x,y) 
        
    elif sweep_num == 2: #0 -> 180
        if distance >= 150:
            distance = 150
        elif distance <= 0:
            distance = 0   
        multiangle = math.radians(multiangle)
        x = 400 + (distance * math.cos(multiangle)) #trig on the point
        y = 200 + (distance * math.sin(multiangle))
        plot_point(x,y)  
    
    elif sweep_num == 3: #90 -> 270
        if distance >= 150:
            distance = 150
        elif distance <= 0:
            distance = 0   
        multiangle = abs(270 - multiangle)
        multiangle = math.radians(multiangle)
        x = 400 + (distance * math.cos(multiangle)) #trig on the point
        y = 200 - (distance * math.sin(multiangle))
        plot_point(x,y) 
    
    else:
        print ("Error sweep value not valid")    

    draw_buttons()

##############################################
#              BUTTONS AND LINES             #
##############################################

def button(x,y,w,h,r1,g1,b1,r2,g2,b2,m,name,yname,xname,mouse0,mouse1,click,font):
    global mode,sensor
    if x+w > mouse0 > x and y+h > mouse1 > y:
        pygame.draw.rect(screen, (r1,g1,b1), (x, y, w, h))
        if click == 1:
            pygame.event.wait()
            if m == "ir" or m == "us":
                sensor = m
            else:
                mode = m
    else:
        pygame.draw.rect(screen, (r2,g2,b2), (x, y, w, h))
    button = font.render(name, True, (0,0,0))
    screen.blit(button, (yname,xname))



def draw_buttons(): #check poisiton values so in middle of boxes
    global mode,buttonfont
   
    mouse = pygame.mouse.get_pos()
    click = pygame.mouse.get_pressed()    
    if mode == "radar": #dont do anything until mouse released
        button(5,10,40,40,178,102,255,204,153,255,"tape","TAPE",10,25,mouse[0],mouse[1],click[0],buttonfont)
        button(5,60,40,40,0,255,128,153,255,204,"overhead","OVER",11,75,mouse[0],mouse[1],click[0],buttonfont)
        button(5,110,40,40,255,102,102,255,153,153,"radar","RADAR",8,125,mouse[0],mouse[1],click[0],buttonfont)
        button(5,160,40,40,255,153,51,255,204,153,"multi","MULTI",10,175,mouse[0],mouse[1],click[0],buttonfont)
        button(55,10,40,40,51,153,255,153,204,255,"ir","IR",70,25,mouse[0],mouse[1],click[0],buttonfont)
        button(55,60,40,40,255,255,0,255,255,204,"us","US",69,75,mouse[0],mouse[1],click[0],buttonfont)
    else:
        button(5,150,40,40,178,102,255,204,153,255,"tape","TAPE",10,165,mouse[0],mouse[1],click[0],buttonfont)
        button(5,100,40,40,255,255,0,255,255,204,"us","US",19,115,mouse[0],mouse[1],click[0],buttonfont)
        button(5,200,40,40,0,255,128,153,255,204,"overhead","OVER",11,215,mouse[0],mouse[1],click[0],buttonfont)
        button(5,250,40,40,255,102,102,255,153,153,"radar","RADAR",8,265,mouse[0],mouse[1],click[0],buttonfont)
        button(5,50,40,40,51,153,255,153,204,255,"ir","IR",20,65,mouse[0],mouse[1],click[0],buttonfont)
        button(5,300,40,40,255,153,51,255,204,153,"multi","MULTI",10,315,mouse[0],mouse[1],click[0],buttonfont)

def plot_point(xcenter,ycenter): #draws cross point
    pygame.draw.line(screen, (255,0,0),(xcenter - 2.5,ycenter - 2.5),(xcenter + 2.5,ycenter + 2.5),2)
    pygame.draw.line(screen, (255,0,0),(xcenter + 2.5,ycenter -2.5),(xcenter - 2.5,ycenter + 2.5),2)

def draw_lines(lines): #draw overheadlines
    pygame.draw.lines(screen, (255,51,51), False, lines,2)

##############################################
#                  GLOBALS                   #
##############################################
count = 0
pygame.init() #essential 
colour = (255,255,255) #background colour
buttonfont = pygame.font.SysFont("Arial", 10) #buttons font
font = pygame.font.SysFont("Arial", 25) #number font 
sensor = "ir" #initial sensor
mode = "tape" #initial mode 
ir_raw = 0 #current raw ir
us_raw = 0 #current raw us
ir = 0 #current ir
us = 0 #current us
angle = 0 #current angle
max_rot = 0 #max rotation - multiview and radar mode
min_rot = 0 #min rotation - multiview and radar mode
cal_point = 0 #for cal mode a single calibration point
sweep_num = 0 #sweep its on for multiview mode
a = 0 #overhead x val point
line = 0
lines = []   #line in saved file
pointlist = [] #points for line to draw between
screen=pygame.display.set_mode((800,400)) #set and lock display size do not change as math is dependant on size
pygame.mouse.set_visible(1) #enables use of mouse
#start_threading()
#Serialdata()
main()

