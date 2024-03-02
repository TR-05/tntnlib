import matplotlib.pyplot as plt
import matplotlib.patches as patches
import matplotlib.transforms as transforms
import matplotlib.image as mpimg
import numpy as np
from scipy.ndimage import zoom

time = 20
refreshTime = 10/1000
num = int(time / refreshTime)

bot = mpimg.imread("logo4.png")
scale_factor = 0.8
bot = zoom(bot, (scale_factor, scale_factor, 1))


img = plt.imread("match field.png")
img = np.clip(img, 0, 1)
# Create a figure and an axis
fig, axs = plt.subplots(nrows=2, ncols=2)
axs[1,1].imshow(img, extent=[0, 144, 0, 144], zorder=0)
m, n = 2, 2
# Define the labels for each subplot
x_labels = [['Time (s)', 'Time (s)'], ['Time (s)', 'x (inch)']]
y_labels = [['d1', 'd2'], ['d3', 'y (inch)']]

for i in range(m):
    for j in range(n):
        axs[i, j].set_xlabel(x_labels[i][j])
        axs[i, j].set_ylabel(y_labels[i][j])
        axs[i, j].grid(True)



# Initialize your data
tData = np.linspace(-time, 0, num).tolist()
backUptData = np.linspace(-time, 0, num).tolist()

p1Data = np.linspace(0, 0, num).tolist()
p2Data = np.linspace(0, 0, num).tolist()
p3Data = np.linspace(0, 0, num).tolist()
backUpP1Data = np.linspace(0, 0, num).tolist()
backUpP2Data = np.linspace(0, 0, num).tolist()
backUpP3Data = np.linspace(0, 0, num).tolist()

xData = [0]
yData = [0]
heading = 0
coords, = axs[1,1].plot(xData, yData, linestyle='solid', color='black', linewidth=2, zorder=1)

def updateLine1(newT, newP1, newP2, newP3, x, y, h):
    global backUptData, backUpP1Data, heading
    heading = h
    backUptData.append(newT)
    backUpP1Data.append(newP1)
    backUpP2Data.append(newP2)
    backUpP3Data.append(newP3)
    xData.append(x)
    yData.append(y)


class Line:
    def __init__(self, height, plotx, ploty):
        global num
        self.height = height
        self.y_data=[height]*num
        self.x_data=[0]*num
        self.line, = axs[plotx,ploty].plot(self.x_data, self.y_data,linestyle='dashed', color='black')
        self.line.set_xdata(self.x_data)
        self.line.set_ydata(self.y_data)
    def update_line(self):
        global tData
        self.line.set_xdata(tData)
        self.line.set_ydata([self.height]*len(tData))


class dataOverTimeLine:
    def __init__(self, plotx, ploty):
        global num, tData
        #self.line, = axs[plotx,ploty].plot(tData, np.linspace(0, 0, num).tolist(), 'o', color='red', markersize=2)
        self.line, = axs[plotx,ploty].plot(tData, np.linspace(0, 0, num).tolist(),linestyle='solid', color='red', linewidth=2)
        self.max_y = 1
        self.min_y = -1
    def update_line(self, data):
        global tData
        self.line.set_xdata(tData)
        self.line.set_ydata(data)
        # Update max_y if necessary
        current_max_y = max(data)
        current_min_y = min(data)
        if current_max_y > self.max_y:
            self.max_y = current_max_y
        if current_min_y < self.min_y:
            self.min_y = current_min_y


# Create a line object that will be updated
line1 = dataOverTimeLine(0,0)
line2 = dataOverTimeLine(1,0)
line3 = dataOverTimeLine(0,1)

# Predefined values for the first argument of each Line
predefined_values = [[[0, 2950, 0], [0, 2, 0]], [[0, 12, 0], [0, 0, 0]]]

x_axes = []
targets = []
targetBs = []

for i in range(m):
    for j in range(n):
        x_axes.append(Line(predefined_values[i][j][0], i, j))
        targets.append(Line(predefined_values[i][j][1], i, j))
        targetBs.append(Line(predefined_values[i][j][2], i, j))




def limitData():
    global num, time, tData, p1Data, backUptData, backUpP1Data 
    while backUptData[-1] - backUptData[-num] > time:
        num = num - 1
    while backUptData[-1] - backUptData[-num] < time:
        num = num + 1
    tData = backUptData[-num:]
    p1Data = backUpP1Data[-num:]
    p2Data = backUpP2Data[-num:]
    p3Data = backUpP3Data[-num:]

    #print(num)
    #print(f"tData ({backUptData[-1]:.3f}, {backUptData[-2]:.3f}, {backUptData[-3]:.3f}, {backUptData[-4]:.3f}, {backUptData[-5]:.3f}, {backUptData[-6]:.3f})")
    line1.update_line(p1Data)
    line2.update_line(p2Data)
    line3.update_line(p3Data)


square = patches.Rectangle((0.5, 0.5), 0.1, 0.1, angle=heading, fill=False)
trans = transforms.Affine2D().rotate_deg(heading).translate(0, 0)
bot_image = axs[1, 1].imshow(np.clip(bot, 0, 1), transform=trans + axs[1, 1].transData, zorder=2)

def update():
    global max_y, min_y # Declare max_y as global so we can modify it
    global num, time, square, bot_image
    limitData()

    for x_axis in x_axes:
        x_axis.update_line()
    for target in targets:
        target.update_line()
    for targetB in targetBs:
        targetB.update_line()

    # Assuming lines is a 2D list containing line1, line2, and line3
    lines = [[line1, line3], [line2, None]]

    for i in range(2):
        for j in range(2):
            if lines[i][j] is not None:
                axs[i, j].set_ylim(bottom=lines[i][j].min_y*1.2, top=lines[i][j].max_y*1.2)
            axs[i, j].relim()
            axs[i, j].autoscale_view()



    # Remove all patches from the plot
    for patch in axs[1, 1].patches:
        patch.remove()
    length = 20
    width = 20
    #print(f"heading: {heading}")
    # Convert the heading to radians
    heading_rad = np.deg2rad(heading)
    # Calculate the displacement caused by the rotation
    dx = length / 2 * np.cos(heading_rad) - width / 2 * np.sin(heading_rad)
    dy = length / 2 * np.sin(heading_rad) + width / 2 * np.cos(heading_rad)
    # Calculate the lower left corner of the square
    lower_left_x = xData[-1] - dx
    lower_left_y = yData[-1] - dy
    # Create the square
    trans = transforms.Affine2D().rotate_deg(heading).translate(lower_left_x, lower_left_y)
    # Display the image at the specified location and rotation

    coords.set_xdata(xData)
    coords.set_ydata(yData)

    bot_image.remove()
    bot_image = axs[1, 1].imshow(np.clip(bot, 0, 1), transform=trans + axs[1, 1].transData, zorder=2)

    square = patches.Rectangle((lower_left_x, lower_left_y), length, width, angle=heading, 
                           edgecolor='black', facecolor='gray')
    #axs[1,1].add_patch(square)


    # Set the x and y limits for the bottom right plot separately
    axs[1, 1].set_xlim(left=0, right=144)
    axs[1, 1].set_ylim(bottom=0, top=144)

    # Redraw the plot
    plt.draw()
    plt.pause(0.001)