import osmnx as ox
import numpy as np
import csv
import sys

def get_map(x,y,d):
    '''Retreive map from database or from file

    Args:
        x: altitude
        y: longitude
        d: distance from origin

    Returns:
    	None
    '''
    place = None
    distance = 1000
    from_file = False
    filename = None
    place = (x,y)
    distance = d

    graph = ox.graph_from_point(place,network_type='drive',distance=distance)
    nodes, edges = ox.graph_to_gdfs(graph)

    id_index = nodes.columns.get_loc('osmid')
    x_index = nodes.columns.get_loc('x')
    y_index = nodes.columns.get_loc('y')

    nodes_data = nodes.values[:,[id_index,x_index,y_index]]

    #-id, lanes are list when road is 2-way
    id_index = edges.columns.get_loc('osmid')
    lanes_index = edges.columns.get_loc('lanes')
    length_index = edges.columns.get_loc('length')
    #- maxspeed might be nan
    speed_index = edges.columns.get_loc('maxspeed')
    u_index = edges.columns.get_loc('u')
    v_index = edges.columns.get_loc('v')
    oneway_index = edges.columns.get_loc('oneway')

    edges_data = edges.values[:,[id_index,u_index,v_index,length_index,\
            speed_index,oneway_index,lanes_index]]

    #print(int(str(edges_data[1,4]).split()[0]))
    #edges_data[:,4] = np.array([float(''.join(j for j in str(i) if j.isdigit())) for i in edges_data[:,4] if len(i) > 3])
    for i in range(len(edges_data[:,4])):
    	tmp = edges_data[i,4]
    	if isinstance(tmp,list):
    		tmp = tmp[0]
    	if isinstance(tmp,float):
    		edges_data[i,4] = 25 if np.isnan(tmp) else int(tmp)
    	else:
    		tmp = tmp.split()[0]
    		edges_data[i,4] = 25 if tmp=='nan' else int(tmp)

    	edges_data[i,5] = 2 if edges_data[i,5] else 1
    	if str(edges_data[i,6])=='nan':
        	edges_data[i,6] = 2
    	elif isinstance(edges_data[i,6],list):
        	edges_data[i,6] = edges_data[i,6][0]
    	if isinstance(edges_data[i,0],list):
        	edges_data[i,0] = edges_data[i,0][0]

    #print(edges_data[1,4])

    with open('node.csv','w') as f:
    	wr = csv.writer(f)
    	wr.writerows(nodes_data)

    with open('edge.csv','w') as f:
    	wr = csv.writer(f)
    	wr.writerows(edges_data)

if __name__ == '__main__':
	#get_map(47.608013,-122.335167,1500)
	get_map(47.7543,-122.1635,2000)


