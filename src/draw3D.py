import numpy as np
import matplotlib.pyplot as plt
import os
from mpl_toolkits.mplot3d import Axes3D

def read_data(path):
    xl = []
    yl = []
    zl = []
    with open(path, 'r') as file:
        idx = 0
        while True:
            lines = file.readline()
            if not lines:
                break
                pass
            x, y, z = [int(i) for i in lines.split()]
            xl.append(x)
            yl.append(y)
            zl.append(z)
            idx = idx + 1
            if idx == 100000:
                #break
                pass
            pass
        pass
    step = 3
    data = [xl[0::step], yl[0::step], zl[0::step]]
    print(max(xl))
    print(min(xl))
    print(max(yl))
    print(min(yl))
    print(max(zl))
    print(min(zl))
    return data

def draw_3d(data):
    x, y, z = data[0], data[1], data[2]
    ax = plt.subplot(111, projection='3d')
    ax.scatter(x, y, z, cmap='spectral', c='r', s=2, linewidth=0, alpha=1, marker='.')
    ax.set_xlim(-20, 300)
    ax.set_ylim(-20, 300)
    ax.set_zlim(-20, 300)
    ax.set_zlabel('Z')
    ax.set_ylabel('Y')
    ax.set_xlabel('X')
    plt.savefig(os.getcwd() + '/3dfig.png');
    plt.show()

if __name__ == '__main__':
    
    path = os.getcwd() + '/params/3dPoint.txt'
    print(path)
    data = read_data(path)
    print(len(data[0]))
    print('read data done')
    draw_3d(data)
    print('done')
