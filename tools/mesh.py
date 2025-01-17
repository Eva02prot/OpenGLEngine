#!/usr/bin/env python
# coding: utf-8

import sys
import os
import shutil
import struct


"""
此工具用来自动生成公共的mesh 提供给c++使用
目前导出的mesh:
1.  quad
2.  cube
3.  plane
"""

target = "../resources/engine/common/"

def writef(f,v):
	p=struct.pack("f",v)
	f.write(p)

def writei(f,v):
	p=struct.pack("I",v)
	f.write(p)



def writeh(f, indices, verts, atype):
    f.write(struct.pack('h', 1))
    writei(f, verts)
    writei(f, atype)
    writei(f, indices)


def readh(f):
    v=f.read(2)
    return struct.unpack("h",v)[0]

def readi(f):
	v=f.read(4)
	return struct.unpack("I",v)[0]

def readf(f):
	v=f.read(4)
	return struct.unpack("f",v)[0]


quad = [	\
        # positions    # texCoords	\
        0.6, -0.6, 0.0, 1.0,	\
        0.6, -1.0, 0.0, 0.0,	\
        1.0, -1.0, 1.0, 0.0,	\
        	
        0.6, -0.6, 0.0, 1.0,	\
        1.0, -1.0, 1.0, 0.0,	\
        1.0, -0.6, 1.0, 1.0,	\
    ]

quad2 = [    \
        # positions    # texCoords  \
        -1.0, 1.0, 0.0, 1.0,    \
        -1.0,-1.0, 0.0, 0.0,    \
        1.0, -1.0, 1.0, 0.0,    \
            
        -1.0, 1.0, 0.0, 1.0,    \
        1.0, -1.0, 1.0, 0.0,    \
        1.0,  1.0, 1.0, 1.0,    \
    ]


cube = [	\
    	# //position, texcoord,  normal	\
        # back face	\
        -1.0, -1.0, -1.0,  0.0, 0.0,    0.0,  0.0, -1.0, # bottom-left	\
        1.0,   1.0, -1.0,  1.0, 1.0,    0.0,  0.0, -1.0, # top-right	\
        1.0,  -1.0, -1.0,  1.0, 0.0,    0.0,  0.0, -1.0, # bottom-right	\
        1.0,   1.0, -1.0,  1.0, 1.0,    0.0,  0.0, -1.0, # top-right	\
        -1.0, -1.0, -1.0,  0.0, 0.0,    0.0,  0.0, -1.0, # bottom-left	\
        -1.0,  1.0, -1.0,  0.0, 1.0,    0.0,  0.0, -1.0, # top-left	\
        # front face	\
        -1.0, -1.0,  1.0,  0.0, 0.0,    0.0,  0.0,  1.0, # bottom-left	\
        1.0,  -1.0,  1.0,  1.0, 0.0,    0.0,  0.0,  1.0, # bottom-right	\
        1.0,   1.0,  1.0,  1.0, 1.0,    0.0,  0.0,  1.0, # top-right	\
        1.0,   1.0,  1.0,  1.0, 1.0,    0.0,  0.0,  1.0, # top-right	\
        -1.0,  1.0,  1.0,  0.0, 1.0,    0.0,  0.0,  1.0, # top-left	\
        -1.0, -1.0,  1.0,  0.0, 0.0,    0.0,  0.0,  1.0, # bottom-left	\
        # left face	\
        -1.0,  1.0,  1.0, 1.0,  0.0,    -1.0,  0.0,  0.0, # top-right	\
        -1.0,  1.0, -1.0, 1.0,  1.0,    -1.0,  0.0,  0.0, # top-left	\
        -1.0, -1.0, -1.0, 0.0,  1.0,    -1.0,  0.0,  0.0, # bottom-left	\
        -1.0, -1.0, -1.0, 0.0,  1.0,    -1.0,  0.0,  0.0, # bottom-left	\
        -1.0, -1.0,  1.0, 0.0,  0.0,    -1.0,  0.0,  0.0, # bottom-right	\
        -1.0,  1.0,  1.0, 1.0,  0.0,    -1.0,  0.0,  0.0, # top-right	\
        # right face	\
        1.0,  1.0,  1.0,   1.0, 0.0,    1.0,  0.0,  0.0, # top-left	\
        1.0, -1.0, -1.0,   0.0, 1.0,    1.0,  0.0,  0.0, # bottom-right	\
        1.0,  1.0, -1.0,   1.0, 1.0,    1.0,  0.0,  0.0, # top-right	\
        1.0, -1.0, -1.0,   0.0, 1.0,    1.0,  0.0,  0.0, # bottom-right	\
        1.0,  1.0,  1.0,   1.0, 0.0,    1.0,  0.0,  0.0, # top-left	\
        1.0, -1.0,  1.0,   0.0, 0.0,    1.0,  0.0,  0.0, # bottom-left	\
        # bottom face	\
        -1.0, -1.0, -1.0,  0.0, 1.0,    0.0, -1.0,  0.0, # top-right	\
        1.0,  -1.0, -1.0,  1.0, 1.0,    0.0, -1.0,  0.0, # top-left	\
        1.0,  -1.0,  1.0,  1.0, 0.0,    0.0, -1.0,  0.0, # bottom-left	\
        1.0,  -1.0,  1.0,  1.0, 0.0,    0.0, -1.0,  0.0, # bottom-left	\
        -1.0, -1.0,  1.0,  0.0, 0.0,    0.0, -1.0,  0.0, # bottom-right	\
        -1.0, -1.0, -1.0,  0.0, 1.0,    0.0, -1.0,  0.0, # top-right	\
        # top face	\
        -1.0,  1.0, -1.0,  0.0, 1.0,    0.0,  1.0,  0.0, # top-left	\
        1.0,   1.0,  1.0,  1.0, 0.0,    0.0,  1.0,  0.0, # bottom-right	\
        1.0,   1.0, -1.0,  1.0, 1.0,    0.0,  1.0,  0.0, # top-right	\
        1.0,   1.0,  1.0,  1.0, 0.0,    0.0,  1.0,  0.0, # bottom-right	\
        -1.0,  1.0, -1.0,  0.0, 1.0,    0.0,  1.0,  0.0, # top-left	\
        -1.0,  1.0,  1.0,  0.0, 0.0,    0.0,  1.0,  0.0, # bottom-left	\
    ]


plane = [	\
        # // positions       // texCoords  // normals          \
        25.0, -0.5,   25.0,   25.0,  0.0,	0.0, 1.0, 0.0,  \
        -25.0, -0.5,  25.0,   0.0,  0.0,	0.0, 1.0, 0.0,  \
        -25.0, -0.5, -25.0,   0.0, 25.0,    0.0, 1.0, 0.0,	\
        	\
        25.0, -0.5,  25.0,   25.0,  0.0,    0.0, 1.0, 0.0,	\
        -25.0, -0.5, -25.0,  0.0,  25.0,	0.0, 1.0, 0.0,  \
        25.0, -0.5, -25.0,   25.0, 25.0,    0.0, 1.0, 0.0,	\
    ]

grass = [ \
        0.0,  0.5, 0.0, 0.0, 0.0,   \
        0.0, -0.5, 0.0, 0.0, 1.0,   \
        1.0, -0.5, 0.0, 1.0, 1.0,   \
        0.0,  0.5, 0.0, 0.0, 0.0,   \
        1.0, -0.5, 0.0, 1.0, 1.0,   \
        1.0,  0.5, 0.0, 1.0, 0.0,   \
    ]


if os.path.exists(target):
    shutil.rmtree(target,True)
    
os.makedirs(target)


def writequad(name, quad):
    f = open(target+name,"wb")
    writeh(f, 0, 6, 0x0012)
    idx = 0
    for x in range(0, 6):
        writef(f,quad[idx])
        idx=idx+1
        writef(f,quad[idx])
        idx=idx+1
        writef(f,quad[idx])
        idx=idx+1
        writef(f,quad[idx])
        idx=idx+1
    f.close()

writequad("quad.mesh", quad)
writequad("quad2.mesh", quad2)



f = open(target+"cube.mesh","wb")
writeh(f, 0, 36, 0x0111)
idx = 0
for x in range(0,36):
	writef(f,cube[idx])
	idx=idx+1
	writef(f,cube[idx])
	idx=idx+1
	writef(f,cube[idx])
	idx=idx+1
	writef(f,cube[idx])
	idx=idx+1
	writef(f,cube[idx])
	idx=idx+1
	writef(f,cube[idx])
	idx=idx+1
	writef(f,cube[idx])
	idx=idx+1
	writef(f,cube[idx])
	idx=idx+1
f.close()

f = open(target+"plane.mesh","wb")
writeh(f, 0, 6, 0x0111)
idx = 0
for x in range(0,6):
	writef(f,plane[idx])
	idx=idx+1
	writef(f,plane[idx])
	idx=idx+1
	writef(f,plane[idx])
	idx=idx+1
	writef(f,plane[idx])
	idx=idx+1
	writef(f,plane[idx])
	idx=idx+1
	writef(f,plane[idx])
	idx=idx+1
	writef(f,plane[idx])
	idx=idx+1
	writef(f,plane[idx])
	idx=idx+1
f.close()

f = open(target+"grass.mesh","wb")
writeh(f, 0, 6, 0x0011)
idx = 0
for x in range(0,6):
    writef(f,grass[idx])
    idx=idx+1
    writef(f,grass[idx])
    idx=idx+1
    writef(f,grass[idx])
    idx=idx+1
    writef(f,grass[idx])
    idx=idx+1
    writef(f,grass[idx])
    idx=idx+1
f.close()


#  for test
f = open(target+"plane.mesh", 'rb')
lod = readh(f)
num=readi(f)
ty = readi(f)
inds=readi(f)
print("lod",lod,"inds:",inds,"num:",num,"type:",ty)
for x in range(0,num):
	a1 = readf(f)
	a2 = readf(f)
	a3 = readf(f)
	a4 = readf(f)
	a5 = readf(f)
	a6 = readf(f)
	a7 = readf(f)
	a8 = readf(f)
	print(a1,a2,a3,a4,a5,a6,a7,a8)
f.close()