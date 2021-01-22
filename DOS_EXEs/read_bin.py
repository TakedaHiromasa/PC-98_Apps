# coding: utf-8
import numpy as np

f = open('C.bin', 'rb')
data = np.fromfile(f, dtype=np.float32)
data = data.reshape(512, 512)
print(data)