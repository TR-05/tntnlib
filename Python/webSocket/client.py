#!/usr/bin/env python
import websockets
import re
import asyncio
import visualizer
import math
import time

start_time = time.time()  # Save the current time at the start of the program


t = 0
d1 = 0
d2 = 0
d3 = 0
x = 0
y = 0
h = 0
# Define an asynchronous function that updates the data
async def update_data():
    while True:
        # Add a new value to the data
        global t, d1, d2, d3, x, y, start_time, visualizer
        end_time = time.time()  # Save the current time at the end of the program
        '''t = end_time - start_time
        d1 = 3600* abs(math.sin(t * (1/4 * math.pi)))
        d2 = 12 * abs(math.sin(t * (1/4 * math.pi)))
        d3 = 1.8* abs(math.sin(t * (1/4 * math.pi)))
        x = 144 * abs(math.sin(t * (1/4 * math.pi)))
        y = 144 * abs(math.cos(t * (1/4 * math.pi)))
        newxterm = 144 * abs(math.sin(t * (1/4 * math.pi)))/abs(math.sin(t * (1/4 * math.pi)))*math.cos(t*math.pi/4)* math.pi/4
        newyterm = -144 * abs(math.cos(t * (1/4 * math.pi)))/abs(math.cos(t * (1/4 * math.pi)))*math.sin(t*math.pi/4)* math.pi/4
        h = math.atan2(newyterm, newxterm) * 180 / math.pi - 90'''
        visualizer.updateLine1(t, d1, d2, d3, x, y, h)
        # Update the plot
        visualizer.update()
        # Wait for a bit
        await asyncio.sleep(visualizer.refreshTime)


def is_float_sequence(s):
    # The regular expression pattern for a sequence of four floats, including negative floats
    #pattern = r'-?\d*(\.\d+)?, -?\d*(\.\d+)?, -?\d*(\.\d+)?, -?\d*(\.\d+)?'
    pattern = r'-?\d*(\.\d+)?,-?\d*(\.\d+)?,-?\d*(\.\d+)?,-?\d*(\.\d+)?,-?\d*(\.\d+)?,-?\d*(\.\d+)?,-?\d*(\.\d+)?'
    output = re.search(pattern, s)
    if output != None:
        output = output.group(0)
    if output:
        return output
    else: return None

async def hello():
    
    #uri = "ws://0.0.0.0:7071/vexrobotics.vexcode/device"
    uri = "ws://localhost:7071/vexrobotics.vexcode/device"
    async with websockets.connect(uri) as websocket:
        while True: 
            #print("Connected to server")
            greeting = await websocket.recv()
            #print("yo")
            values_string = greeting.decode('utf-8')
            checked_string = is_float_sequence(values_string)
            if checked_string != None:    
                print("Valid input")
                print(values_string, end='')
            #if True:    
                # Split the string into a list of values
                values_list = checked_string.split(',')
                # Convert the values to float (since they seem to be decimal numbers)
                values_list = [float(value) for value in values_list]
                # Assign the values to a preset list of variables
                global d1, d2, d3, t, x, y, h
                t, d1, d2, d3, x, y, h = values_list
            else:
                print("Invalid input")
                print(values_string)
                pass

