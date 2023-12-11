#import customtkinter
from tkinter import *
from PIL import ImageTk, Image  
import numpy as np

from tkinter import filedialog

# Display the dialog for browsing files.
#filename = filedialog.askopenfilename()
# Print the selected file path.
#print(filename)



    


window = Tk()
window.title("Drag Script")
window.geometry("1000x800")


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
frame = Frame(window, relief='raised', borderwidth=2)
frame.pack(fill=BOTH, expand=YES)
frame.pack_propagate(False)

canvas = Canvas(frame, bg="black", height=750, width=750, cursor="crosshair")
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

p0 = Point("p0",0,60,"green",1,1)
p1 = Point("p1",0,60,"blue",1,1)
p2 = Point("p2",0,60,"red",1,1)
p3 = Point("p3",0,60,"orange",1,1)

number_of_points = 20
points = [0.0 for i in range(number_of_points)]
def generateCubic(p0x,p0y,p1x,p1y,p2x,p2y,p3x,p3y):
    #print(f"({p0x},{p0y}),({p1x},{p1y}),({p2x},{p2y}),({p3x},{p3y}),")
    global number_of_points
    t = np.linspace(0,1,number_of_points)
    x = (1-t)**3*p0x + 3*(1-t)**2*t*p1x + 3*(1-t)*t**2*p2x + t**3*p3x
    y = (1-t)**3*p0y + 3*(1-t)**2*t*p1y + 3*(1-t)*t**2*p2y + t**3*p3y
    for i in range(number_of_points):
        canvas.delete(points[i])
    for i in range(len(x)):
        #print(f"({x[i]},{y[i]}),")
        #position1 = (x[i], y[i])*2
        size = 4
        points[i] = canvas.create_rectangle(x[i]-size, y[i]-size, x[i]+size, y[i]+size, outline="black", fill="red", width=1)
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

resizeButton = Button(master=frame, text="Resize", command=resize_image)
resizeButton.pack()
resizeButton.place(x=0,y=30)


readout0 = 'p0: ({0},{1})'.format(p0.x,p0.y)
readout1 = 'p1: ({0},{1})'.format(p1.x,p1.y)

left_frame = Frame(frame, relief='raised', borderwidth=2)
left_frame.place(relx=0.1, rely=0.5, anchor=CENTER)
Label(left_frame, textvariable=p0ReadOut,fg="black", width=12).pack()
Label(left_frame, textvariable=p1ReadOut,fg="black", width=12).pack()
Label(left_frame, textvariable=p2ReadOut,fg="black", width=12).pack()
Label(left_frame, textvariable=p3ReadOut,fg="black", width=12).pack()

#window.mainloop()
#print(generateCubic(p0.x,p0.y,p1.x,p1.y,p2.x,p2.y,p3.x,p3.y, 20))
generateCubic(p0.x,p0.y,p1.x,p1.y,p2.x,p2.y,p3.x,p3.y)
while loop:
    window.update_idletasks()
    p0ReadOut.set('p0: ({0},{1})'.format(p0.x,p0.y))
    p1ReadOut.set('p1: ({0},{1})'.format(p1.x,p1.y))
    p2ReadOut.set('p2: ({0},{1})'.format(p2.x,p2.y))
    p3ReadOut.set('p3: ({0},{1})'.format(p3.x,p3.y))
    window.update()
    generateCubic(p0.x,p0.y,p1.x,p1.y,p2.x,p2.y,p3.x,p3.y)
    #print(p0,p1,p2,p3)
