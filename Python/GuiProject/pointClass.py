import util
from tkinter import *
class Point():
    x = 0
    y = 0
    def __str__(self):
        return f"{self.name}:({self.x},{self.y})"
    def drag_motion(self, event):
        self.x = event.x
        self.y = event.y
        if self.x > util.canvasWidth:
            self.x = util.canvasWidth
        if self.y > util.canvasHeight:
            self.y = util.canvasHeight
        if self.x < 0:
            self.x = 0
        if self.y < 0:
            self.y = 0
        self.readOut.set('{0:.1f},{1:.1f},  '.format( util.pToX(self.x),util.pToY(self.y) ))
        util.canvas.moveto(self.point, self.x-(self.size/2),self.y-(self.size/2))
    def __init__(self, name, startx, starty, color, size):
        self.x = util.xToP(startx)
        self.y = util.yToP(starty)
        self.name = name
        self.startx = util.xToP(startx)
        self.starty = util.yToP(starty)
        self.tag = name
        self.size = size
        self.point = util.canvas.create_oval(self.x-(size/2),self.y-(size/2),self.x+(size/2),self.y+(size/2),fill=color, tag=self.tag)
        util.canvas.tag_bind(self.tag, "<B1-Motion>", self.drag_motion)
        self.readOut = StringVar()
        self.readOut.set('{0},{1},  '.format(startx,starty))