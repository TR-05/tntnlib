import util
import pointClass
import numpy as np
from tkinter import *
class Curve():
    def __str__(self):
        return f"{self.name}:({self.x},{self.y})"
        
    def __init__(self, name, p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y, copyY, color):
        self.points = [0, 0, 0, 0]
        self.points[0] = pointClass.Point(str(name)+str(0),p0x,p0y,"green",25)
        self.points[1] = pointClass.Point(str(name)+str(1),p1x,p1y, color,25)
        self.points[2] = pointClass.Point(str(name)+str(2),p2x,p2y, color,25)
        self.points[3] = pointClass.Point(str(name)+str(3),p3x,p3y,"red",25)
        self.p0x = p0x
        self.p0y = p0y
        self.p1x = p1x
        self.p1y = p1y
        self.p2x = p2x
        self.p2y = p2y
        self.p3x = p3x
        self.p3y = p3y
        self.fidelity = 25
        self.p0line = 0;
        self.p3line = 0;
        self.interpPoints = [0.0 for i in range(self.fidelity)]
        self.name = name
        self.color = color
        Label(util.left_frame, textvariable=self.points[0].readOut,fg="black", width=20).pack()
        Label(util.left_frame, textvariable=self.points[1].readOut,fg="black", width=20).pack()
        Label(util.left_frame, textvariable=self.points[2].readOut,fg="black", width=20).pack()
        Label(util.left_frame, textvariable=self.points[3].readOut,fg="black", width=20).pack()
        copy = Button(master=util.frame, text=str(name) + " Copy", fg=self.color, command=self.copyButton)
        copy.pack()
        copy.place(x=0,y=copyY)
    def generateCubic(self):
        #print(f"({p0x},{p0y}),({p1x},{p1y}),({p2x},{p2y}),({p3x},{p3y}),")
        t = np.linspace(0,1,self.fidelity)
        x = (1-t)**3*self.points[0].x + 3*(1-t)**2*t*self.points[1].x + 3*(1-t)*t**2*self.points[2].x + t**3*self.points[3].x
        y = (1-t)**3*self.points[0].y + 3*(1-t)**2*t*self.points[1].y + 3*(1-t)*t**2*self.points[2].y + t**3*self.points[3].y
        for i in range(self.fidelity):
            util.canvas.delete(self.interpPoints[i])

        util.canvas.delete(self.p0line)
        util.canvas.delete(self.p3line)
        self.p0line = util.canvas.create_line(self.points[0].x, self.points[0].y, self.points[1].x, self.points[1].y, fill="green", width=3)
        self.p3line = util.canvas.create_line(self.points[3].x, self.points[3].y, self.points[2].x, self.points[2].y, fill="green", width=3)
        for i in range(self.fidelity-1):
            size = 4
            self.interpPoints[i] = util.canvas.create_oval(x[i]-size, y[i]-size, x[i]+size, y[i]+size, outline="black", fill=self.color, width=1)
            util.canvas.delete(self.interpPoints[0])
            util.canvas.delete(self.interpPoints[self.fidelity-1])
        return x,y
    def copyButton(self):
        util.window.clipboard_clear()
        fidelity = StringVar()
        fidelity.set('{0:.0f}'.format(util.number_of_points))
        output = "Path path(" + self.points[0].readOut.get() + self.points[1].readOut.get() + self.points[2].readOut.get() + self.points[3].readOut.get() + str(100) + ");"
        new_line = StringVar()
        new_line.set('chassis.follow(path, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 12, {0:.0f});'.format(3))
        util.window.clipboard_append(output + '\n' + new_line.get())
