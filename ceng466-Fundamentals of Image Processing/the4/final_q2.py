# Seda Civelek
# 2237147

import numpy as np   
from sklearn.cluster import MeanShift, estimate_bandwidth
import matplotlib.image as img
import sys
import os

def main(argv):
    originImg = img.imread(argv[1])
    path =argv[1].split("/")
    name = path[-1].split(".")[0]
    originShape = originImg.shape
    flatImg=np.reshape(originImg, [-1, 3]) 
    bandwidth = estimate_bandwidth(flatImg, quantile=0.1, n_samples=8000)    
    ms = MeanShift(bandwidth = bandwidth, bin_seeding=True)
    ms.fit(flatImg)
    labels=ms.labels_  
    segmentedImg = np.reshape(labels, originShape[:2])
    img.imsave(argv[2]+"/"+name+".png",segmentedImg)
if __name__ =="__main__":
    if not os.path.exists(sys.argv[2]):
        os.mkdir(sys.argv[2])
    main(sys.argv)
    