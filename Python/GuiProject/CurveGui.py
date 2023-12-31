from tkinter import *
from PIL import ImageTk, Image  
import numpy as np
import util
import curveClass

def resize_image(event):
    new_width = event.width
    new_height = event.height
    util.image.resize((new_width, new_height))


colors = ["cyan", "orange", "yellow", "green", "blue", "purple", "red", "magenta"]
curves = [curveClass.Curve("c0", 104.5,126.1,  143.7,128.7,  130.8,108.0,  71.4,95.6, 30, colors[0]) for i in range(1)]
#curves[0] = curveClass.Curve("c0", 12, 12, 49, 65, 95, 65, 132, 12, 30, colors[0])
i = 1;

newy = 60;
def newCurveButton():
    global newy
    global i
    if i > 7:
        return
    newy += 30
    print("new")
    name = StringVar()
    name = "c" + str(i)
    curves.append(curveClass.Curve(name, 5,139, 5,129, 5,119, 5,109, newy, colors[i]))
    i += 1

CurveButton = Button(text="new", command=newCurveButton)
CurveButton.pack()
CurveButton.place(x=0,y=newy)

while util.loop:
    util.window.update_idletasks()
    for c in curves:
        c.generateCubic()
    util.window.update()
