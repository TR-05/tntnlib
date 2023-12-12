#import customtkinter
from tkinter import *
from PIL import ImageTk, Image  
import numpy as np

from tkinter import filedialog

# Display the dialog for browsing files.
#filename = filedialog.askopenfilename()
# Print the selected file path.
#print(filename)



number_of_points = 100

window = Tk()
window.title("Drag Script")
window.geometry("1000x800")
#window.wm_attributes("-transparentcolor", 'purple')

p0ReadOut = StringVar()
p0ReadOut.set('p0: ({0},{1})'.format(0,0))
p1ReadOut = StringVar()
p1ReadOut.set('p1: ({0},{1})'.format(0,0))
p2ReadOut = StringVar()
p2ReadOut.set('p2: ({0},{1})'.format(0,0))
p3ReadOut = StringVar()
p3ReadOut.set('p3: ({0},{1})'.format(0,0))

#frame = Frame(master=window)
#frame.pack(pady=5, padx=5, fill="both", expand=True)
frame = Frame(window, relief='raised', borderwidth=2, bg="gray")
frame.pack(fill=BOTH, expand=YES)
frame.pack_propagate(False)

canvasWidth = 738
canvasHeight = 737
xPixelsToInches = 144.0/canvasWidth
yPixelsToInches = 144.0/canvasHeight

def pToX(p):
    return p*xPixelsToInches
def xToP(x):
    return x/xPixelsToInches
def pToY(p):
    return 144 - p*yPixelsToInches
def yToP(y):
    return (144 - y)/yPixelsToInches

canvas = Canvas(frame, bg="black", height=canvasHeight, width=canvasWidth, cursor="crosshair")
#coord = 10, 50, 240, 210
#arc = canvas.create_arc(coord, start=0, extent=150, fill="red")
#line = canvas.create_line(10,10,200,200,fill='white')
canvas.place(x=100,y=800)
canvas.pack()

fieldImage = Image.open("field.png")
#fieldImage = Image.open("C:/Users/trevo/OneDrive/Documents/GitHub/tntnlib/Python/field.png")
test = ImageTk.PhotoImage(fieldImage)
image = canvas.create_image(0, 0, anchor=NW, image=test)

def resize_image(event):
    new_width = event.width
    new_height = event.height
    image.resize((new_width, new_height))





#label1 = Label(frame, image=test)
#label1.image = test
# Position image
#label1.place(x=80, y=20, relwidth=1, relheight=1)
#label1.bind('<Configure>', resize_image)

center_frame = Frame(frame, relief='raised', borderwidth=2)
center_frame.place(relx=0.5, rely=0.5, anchor=CENTER)

#Label(center_frame, text='Full Name', width=8).pack()
#Label(center_frame, text='Education', width=8).pack()

class Point():
    def __str__(self):
        return f"{self.name}:({self.x},{self.y})"
    #def drag_start(self, event):
        print("drag_start")
        self.startx = self.x
        self.starty = self.y
    def drag_motion(self, event):
        widget = event.widget
        global canvasHeight
        global canvasWidth
        self.x = event.x
        self.y = event.y
        #self.x = widget.winfo_x() - self.startx + event.x
        #self.y = widget.winfo_y() - self.starty + event.y
        if self.x > canvasWidth:
            self.x = canvasWidth
        if self.y > canvasHeight:
            self.y = canvasHeight
        if self.x < 0:
            self.x = 0
        if self.y < 0:
            self.y = 0
        #print(event.x, event.y)
        canvas.moveto(self.point, self.x-(self.size/2),self.y-(self.size/2))
        #widget.place(x=self.x,y=self.y)
    def __init__(self, name, startx, starty, color, size):
        self.x = xToP(startx)
        self.y = yToP(starty)
        self.name = name
        self.startx = xToP(startx)
        self.starty = yToP(starty)
        self.tag = name
        self.size = size
        self.point = canvas.create_oval(self.x-(size/2),self.y-(size/2),self.x+(size/2),self.y+(size/2),fill=color, tag=self.tag)
        #canvas.coords(self.point, self.x-(size/2),self.y-(size/2),self.x+(size/2),self.y+(size/2))
        #canvas.tag_bind(self.tag, "<Button-1>", self.drag_start)
        canvas.tag_bind(self.tag, "<B1-Motion>", self.drag_motion)
        #canvas.delete(self.point)
    
        #self.point = canvas.create_oval(self.x-(size/2),self.y-(size/2),self.x+(size/2),self.y+(size/2),fill="green", tag="point")
        width = 1
        height = 1
        #label = Label(master=canvas,bg=color,width=width,height=height)
        #label.place(x=startx,y=starty)
        #label.bind("<Button-1>",self.drag_start)
        #label.bind("<B1-Motion>",self.drag_motion)

p0 = Point("p0",12,12,"green",25)
p1 = Point("p1",49,65,"purple",25)
p2 = Point("p2",95,65,"purple",25)
p3 = Point("p3",132,12,"red",25)

points = [0.0 for i in range(number_of_points)]
p0line = 0;
p3line = 0;
def generateCubic(p0x,p0y,p1x,p1y,p2x,p2y,p3x,p3y):
    #print(f"({p0x},{p0y}),({p1x},{p1y}),({p2x},{p2y}),({p3x},{p3y}),")
    global number_of_points
    global p0line
    global p3line
    t = np.linspace(0,1,number_of_points)
    x = (1-t)**3*p0x + 3*(1-t)**2*t*p1x + 3*(1-t)*t**2*p2x + t**3*p3x
    y = (1-t)**3*p0y + 3*(1-t)**2*t*p1y + 3*(1-t)*t**2*p2y + t**3*p3y
    for i in range(number_of_points):
        canvas.delete(points[i])

    canvas.delete(p0line)
    canvas.delete(p3line)
    p0line = canvas.create_line(p0x, p0y, p1x, p1y, fill="green", tag="line1", width=3)
    p3line = canvas.create_line(p3x, p3y, p2x, p2y, fill="green", tag="line1", width=3)
    for i in range(number_of_points-1):
        size = 4
        points[i] = canvas.create_oval(x[i]-size, y[i]-size, x[i]+size, y[i]+size, outline="black", fill="cyan", width=1)
        canvas.delete(points[0])
        canvas.delete(points[number_of_points-1])
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

readout0 = 'p0: ({0},{1})'.format(p0.x,p0.y)
readout1 = 'p1: ({0},{1})'.format(p1.x,p1.y)

left_frame = Frame(frame, relief='raised', borderwidth=2)
left_frame.place(relx=0.01, rely=0.5, anchor=W)
Label(left_frame, textvariable=p0ReadOut,fg="black", width=20).pack()
Label(left_frame, textvariable=p1ReadOut,fg="black", width=20).pack()
Label(left_frame, textvariable=p2ReadOut,fg="black", width=20).pack()
Label(left_frame, textvariable=p3ReadOut,fg="black", width=20).pack()




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
#mouseLabel.place(x=0,y=100)

def copyButton():
    window.clipboard_clear()
    fidelity = StringVar()
    fidelity.set('{0:.0f}'.format(number_of_points))
    output = "Path path(" + p0ReadOut.get() + p1ReadOut.get() + p2ReadOut.get() + p3ReadOut.get() + fidelity.get()+ ");"
    new_line = StringVar()
    new_line.set('chassis.follow(path, false, 9, 12, lkp, lki, lkd, akp, aki, akd, 0, 12, 12, {0:.0f});'.format(3))
    #line = '\n'.join(output, new_line)
    #window.clipboard_append(output)
    window.clipboard_append(output + '\n' + new_line.get())



copy = Button(master=frame, text="Copy", command=copyButton)
copy.pack()
copy.place(x=0,y=30)

#window.mainloop()
#print(generateCubic(p0.x,p0.y,p1.x,p1.y,p2.x,p2.y,p3.x,p3.y, 20))
generateCubic(p0.x,p0.y,p1.x,p1.y,p2.x,p2.y,p3.x,p3.y)
while loop:
    generateCubic(p0.x,p0.y,p1.x,p1.y,p2.x,p2.y,p3.x,p3.y)
    window.update_idletasks()
    p0ReadOut.set('{0:.1f},{1:.1f},  '.format(pToX(p0.x), pToY(p0.y)))
    p1ReadOut.set('{0:.1f},{1:.1f},  '.format(pToX(p1.x), pToY(p1.y)))
    p2ReadOut.set('{0:.1f},{1:.1f},  '.format(pToX(p2.x), pToY(p2.y)))
    p3ReadOut.set('{0:.1f},{1:.1f},  '.format(pToX(p3.x), pToY(p3.y)))
    window.update()
    #print(p0,p1,p2,p3)
