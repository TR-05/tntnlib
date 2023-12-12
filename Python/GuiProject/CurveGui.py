from tkinter import *
from PIL import ImageTk, Image  
import numpy as np
import util
import curveClass

def resize_image(event):
    new_width = event.width
    new_height = event.height
    util.image.resize((new_width, new_height))

curve1 = curveClass.Curve("c1", 12, 12, 49, 65, 95, 65, 132, 12, 100, 30)
curve2 = curveClass.Curve("c2", 12, 100, 49, 100, 95, 100, 132, 100, 100, 70)



while util.loop:
    util.window.update_idletasks()
    curve1.generateCubic()
    curve2.generateCubic()
    util.window.update()
