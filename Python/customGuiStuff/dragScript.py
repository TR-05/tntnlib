#import customtkinter
from tkinter import *
import numpy as np

window = Tk()
window.geometry("800x500")

frame = Frame(master=window)
frame.pack(pady=5, padx=5, fill="both", expand=True)


class Point():
    def __str__(self):
        return f"{self.name}:({self.x},{self.y})"
    def drag_start(self, event):
        self.startx = event.x
        self.starty = event.y
    def drag_motion(self, event):
        widget = event.widget
        self.x = widget.winfo_x() - self.startx + event.x
        self.y = widget.winfo_y() - self.starty + event.y
        widget.place(x=self.x,y=self.y)
    def __init__(self, name, startx, starty, color, width, height):
        self.x = startx
        self.y = starty
        self.name = name
        self.startx = startx
        self.starty = starty
        label = Label(master=frame,bg=color,width=width,height=height)
        label.place(x=startx,y=starty)
        label.bind("<Button-1>",self.drag_start)
        label.bind("<B1-Motion>",self.drag_motion)

p0 = Point("p0",0,0,"green",1,1)
p1 = Point("p1",0,20,"blue",1,1)
p2 = Point("p2",20,0,"red",1,1)
p3 = Point("p3",20,20,"orange",1,1)

def generateCubic(p0x,p0y,p1x,p1y,p2x,p2y,p3x,p3y, number_of_points):
    print(f"({p0x},{p0y}),({p1x},{p1y}),({p2x},{p2y}),({p3x},{p3y}),")
    t = np.linspace(0,1,number_of_points)
    x = (1-t)**3*p0x + 3*(1-t)**2*t*p1x + 3*(1-t)*t**2*p2x + t**3*p3x
    y = (1-t)**3*p0y + 3*(1-t)**2*t*p1y + 3*(1-t)*t**2*p2y + t**3*p3y
    for i in range(len(x)):
        print(f"({x[i]},{y[i]}),")
    return x,y
    
loop = True 
def exitButton():
    global loop
    loop = False
    window.quit()
    print("Exit")

button = Button(master=frame, text="Exit", command=exitButton)
button.pack(pady=12, padx=10)
button.place(x=0,y=0)

#window.mainloop()
#print(generateCubic(p0.x,p0.y,p1.x,p1.y,p2.x,p2.y,p3.x,p3.y, 20))
generateCubic(p0.x,p0.y,p1.x,p1.y,p2.x,p2.y,p3.x,p3.y, 20)
while loop:
    window.update()
    print(p0,p1,p2,p3)
