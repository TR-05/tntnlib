#!/usr/bin/env python
import websockets
import re
import asyncio
import visualizer



x = 0
y = 0
h = 0
t = 0

# Define an asynchronous function that updates the data
async def update_data():
    while True:
        # Add a new value to the data
        newx = x
        newy = y
        newh = h
        newt = t
        #data.append(random.random())
        visualizer.xData.append(newt)
        visualizer.yData.append(h)

        # Update the plot
        visualizer.update()

        # Wait for a bit
        await asyncio.sleep(visualizer.refreshTime)


def is_float_sequence(s):
    # The regular expression pattern for a sequence of four floats, including negative floats
    pattern = r'-?\d*(\.\d+)?,-?\d*(\.\d+)?,-?\d*(\.\d+)?,-?\d*(\.\d+)?'
    output = re.search(pattern, s).group(0)
    if output:
        return output
    else: return None

async def hello():
    uri = "ws://localhost:7071/vexrobotics.vexcode/device"
    async with websockets.connect(uri) as websocket:
        #await websocket.send(name)
        #print(f">>> {name}")
        while True: 
            greeting = await websocket.recv()
            values_string = greeting.decode('utf-8')
            checked_string = is_float_sequence(values_string)
            if checked_string != None:    
                print("Valid input")
                print(values_string)
            #if True:    
                # Split the string into a list of values
                values_list = checked_string.split(',')
                # Convert the values to float (since they seem to be decimal numbers)
                values_list = [float(value) for value in values_list]
                # Assign the values to a preset list of variables
                global x, y, h, t
                x, y, h, t = values_list
                # Now you can use the variables
                #print("\n\n")
                #print(f"{x}")  # Outputs: 4.0
                #print(y)  # Outputs: 7.0
                #print(h)  # Outputs: 2.5
                #print(t)  # Outputs: 7.0
            else:
                print("Invalid input")
                print(values_string)
                pass
if __name__ == "__main__":
    asyncio.run(hello())

