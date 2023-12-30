from tkinter import*
import tkinter
from PIL import Image, ImageTk

import curveClass
fieldwidth = 500
fieldheight = 500
offset = 200
canvasWidth = fieldwidth + offset
canvasHeight = fieldheight + offset
xPixelsToInches = 144.0/fieldwidth
yPixelsToInches = 144.0/fieldheight

def pToX(p):
    return (p - (offset/2))*xPixelsToInches
def xToP(x):
    return x/xPixelsToInches + (offset/2)
def pToY(p):
    return 144 - (p - (offset/2))*yPixelsToInches
def yToP(y):
    return (144 - y)/yPixelsToInches + (offset/2)

number_of_points = 100
window = Tk()
window.title("Drag Script")
window.geometry("1000x800")
#window.wm_attributes("-transparentcolor", 'purple')

frame = Frame(window, relief='raised', borderwidth=2, bg="gray")
frame.pack(fill=BOTH, expand=YES)
frame.pack_propagate(False)

canvas = Canvas(frame, bg="black", height=canvasHeight, width=canvasWidth, cursor="crosshair")
canvas.place(x=100,y=800)
canvas.pack()

left_frame = Frame(frame, relief='raised', borderwidth=2)
left_frame.place(relx=0.01, rely=0.5, anchor=W)

fieldImage = Image.open("skills field.png")
fieldImage = fieldImage.resize((fieldwidth,fieldheight))
test = ImageTk.PhotoImage(fieldImage)
image = canvas.create_image(100, 100, anchor=NW, image=test)


mouseX = 0
mouseY = 0
mouseReadOut = StringVar()

def getMouseCoords(event):
    global mouseX
    global mouseY
    global mouseReadOut
    mouseX = event.x
    mouseY = event.y
    mouseReadOut.set('mouse: {0:.1f},{1:.1f}'.format(pToX(mouseX), pToY(mouseY)) )

canvas.bind("<Motion>", getMouseCoords)
Label(left_frame, textvariable=mouseReadOut,fg="black", width=20).pack()

loop = True 
def exitButton():
    global loop
    loop = False
    window.quit()
    print("Exit")
button = Button(master=frame, text="Exit", command=exitButton)
button.pack(pady=12, padx=10)
button.place(x=0,y=0)

