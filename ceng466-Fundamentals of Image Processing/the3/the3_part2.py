#members
#Seda Civelek-2237147
#Kağan Erdoğan-2098986


from skimage.color import rgb2gray
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
from PIL import Image
from scipy import ndimage

def segmentation_function(imgpath) :

    img = mpimg.imread(imgpath)
    gray = rgb2gray(img)

    imgNo = imgpath[13]

    grayseg = gray.reshape(gray.shape[0]*gray.shape[1])
    for i in range(grayseg.shape[0]):
        print(i)
        if grayseg[i] > grayseg.mean():
            grayseg[i] = 1
        else:
            grayseg[i] = 0
    gray = grayseg.reshape(gray.shape[0],gray.shape[1])

    outputStr = 'the3_B' + imgNo + '_output.png'
    mpimg.imsave(outputStr,gray,cmap='gray')
