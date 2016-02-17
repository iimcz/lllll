'''
Created on 27. 1. 2016

@author: neneko
'''


import json
import sys

# Width/height in meters
WIDTH=36.5
HEIGHT=27

COLUMNS=5
LIGHTS_IN_COLUMN=17
LIGHTS_GROUP=3
#gap between lights in the same group
LIGHT_GAP=1.0
#gap between light groups
LIGHT_GROUP_GAP=0.35
# gap between lights in a group
COLUMN_GAP=5
# Gap on the bottom
LIGHT_DOWN_GAP=1.3
LIGHT_SIZE=1
DMX_GAP=51
LIGHT_WIDTH=1.5

SCREEN_WIDTH=3840
SCREEN_HEIGHT=2160

START_FROM_TOP = True
# offset for first group (effectively, how many light are missing from first group)
GROUP_OFFSET=1
cfg_root = {
            "window": {
                       "fullscreen": False,
                       "size": {
                                "x": SCREEN_WIDTH,
                                "y": SCREEN_HEIGHT
                        },
                       "background": {
                                      "r": 0,
                                      "g": 25,
                                      "b": 50,
                                      "image": "bg.jpg"
                                      }
                       },
            "scene": {
                      "width": WIDTH,
                      "height": HEIGHT
                      },
            "network": {
                        "address": "0.0.0.0",
                        "port": 6454
                        },
            }

if __name__ == '__main__':
    x_0 = (1 - COLUMNS)*COLUMN_GAP/2
    columns_x = [ x_0 + x * COLUMN_GAP for x in range(0, COLUMNS)]
    if not START_FROM_TOP:
        y_0 = (1 - LIGHTS_IN_COLUMN)*LIGHT_GAP/2 + (1 - int(LIGHTS_IN_COLUMN/LIGHTS_GROUP))*LIGHT_GROUP_GAP + LIGHT_DOWN_GAP
        rows_y = [ y_0 + y * LIGHT_GAP + int((y + GROUP_OFFSET)/LIGHTS_GROUP) * LIGHT_GROUP_GAP for y in range(0, LIGHTS_IN_COLUMN)]
        orientation = 0
    else:
        y_0 = (LIGHTS_IN_COLUMN - 1)*LIGHT_GAP/2 + (int(LIGHTS_IN_COLUMN/LIGHTS_GROUP) - 1)*LIGHT_GROUP_GAP + LIGHT_DOWN_GAP
        rows_y = [ y_0 - y * LIGHT_GAP - int((y + GROUP_OFFSET)/LIGHTS_GROUP) * LIGHT_GROUP_GAP for y in range(0, LIGHTS_IN_COLUMN)]
        orientation = 180
    
    
    #print('Columns',str(columns_x))
    #print('Rows',str(rows_y))
    
    lights = []
    universe = 0
    for x in columns_x:
        dmx = 0
        for y in rows_y:
            lights.append({
                           "type": "elar",
                           "artnet": {
                                      "channel": dmx,
                                      "universe": universe},
                           "position": {
                                        "x": x,
                                        "y": y},
                           "length": LIGHT_SIZE,
                           "ratio": LIGHT_WIDTH,
                           "orientation": orientation
                           })
            dmx = dmx + DMX_GAP
            if dmx + DMX_GAP > 510:
                dmx = 0
                universe = universe + 1
        universe = universe + 1
    
    cfg = cfg_root
    cfg["lights"]=lights
    
    if len(sys.argv) > 1:
        f = open(sys.argv[1],"wt")
        json.dump(cfg, f, indent=2)
    else:
        print(json.dumps(cfg, indent=2))
    
    
    