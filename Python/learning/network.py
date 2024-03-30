import numpy as np
import matplotlib as plt
import pickle

def init__network():
    network = {}
    network['W1'] = np.array([[0.1,0.2,0.3],[0.4,0.5,0.6]])
    network['W2'] = np.array([[0.1,0.4],[0.2,0.5],[0.3,0.6]])
    return network

network = init__network()

print(network)
print(type(network))