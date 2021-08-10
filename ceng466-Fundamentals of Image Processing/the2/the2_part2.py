## Group members
## Seda Civelek 2237147
## Kağan Erdoğan 2098986

import numpy as np
import matplotlib.image as img
import math

#helper functions for grayscaling and mapping
def rgb2gray(rgb):
    r = rgb[:,:,0]
    g = rgb[:,:,1]
    b = rgb[:,:,2]
    grayscale = 0.2989 * r + 0.5870 * g + 0.1140 * b
    return grayscale
def domap(arr, min, max, mapmin,mapmax):
    newarr = np.zeros_like(arr)
    for i in range(arr.shape[0]):
        for j in range(arr.shape[1]):
            newarr[i,j] = (((arr[i,j] - min) / (max - min))) * (mapmax - mapmin) + mapmin
    return newarr
#read images
B1 = img.imread('B1.jpg')
B2 = img.imread('B2.jpg')
B3 = img.imread('B3.jpg')
b1 = rgb2gray(B1)
b2 = rgb2gray(B2)
b3 = rgb2gray(B3)
#B1 height and width
b1height = b1.shape[0]
b1width = b1.shape[1]
#B2 height and width
b2height = b2.shape[0]
b2width = b2.shape[1]
#B3 height and width
b3height = b3.shape[0]
b3width = b3.shape[1]

### B1 EDGE ###
##fourier transform
B1_transform = np.fft.fft2(b1)
#define center of b1
b1_centerh = round(b1height/2)
b1_centerw = round(b1width/2)
#filter mask 
b1mask = np.zeros((b1height,b1width))
b1radius = 110 #chosen by trial and error procedure, lower values have more detail
#construct high pass filter
for h in range(b1height):
    for w in range(b1width):
        distance = np.sqrt((h-b1_centerh)**2+(w-b1_centerw)**2)
        if(distance>b1radius):
            b1mask[h,w] = 1

b1mask = np.fft.ifftshift(b1mask)
#apply high pass filter 
applyFilter = B1_transform*b1mask

#map transformed pixel values
inverseb1 = np.abs(np.fft.ifft2(applyFilter))
inverseb1 = inverseb1/255.0
result = domap(inverseb1,np.min(inverseb1),np.max(inverseb1),0.0,1.0)

img.imsave("B1_output.jpg",result,cmap='gray')

##### B2 EDGE ####

##fourier transform
B2_transform = np.fft.fft2(b2)
#define center of b2
b2_centerh = round(b2height/2)
b2_centerw = round(b2width/2)
#filter mask 
b2mask = np.zeros((b2height,b2width))
b2radius = 100 #chosen by trial and error procedure 
#construct high pass filter
for h in range(b2height):
    for w in range(b2width):
        distance2 = np.sqrt((h-b2_centerh)**2+(w-b2_centerw)**2)
        if(distance2>b2radius):
            b2mask[h,w] = 1

b2mask = np.fft.ifftshift(b2mask)
#apply high pass filter 
applyFilter2 = B2_transform*b2mask

#map transformed pixel values
inverseb2 = np.abs(np.fft.ifft2(applyFilter2))
inverseb2 = inverseb2/255.0
result2 = domap(inverseb2,np.min(inverseb2),np.max(inverseb2),0.0,1.0)
img.imsave("B2_output.jpg",result2,cmap='gray')

##### B3 EDGE ####

##fourier transform
B3_transform = np.fft.fft2(b3)
#define center of b2
b3_centerh = round(b3height/2)
b3_centerw = round(b3width/2)
#filter mask 
b3mask = np.zeros((b3height,b3width))
b3radius = 50 #chosen by trial and error procedure 
#construct high pass filter
for h in range(b3height):
    for w in range(b3width):
        distance3 = np.sqrt((h-b3_centerh)**2+(w-b3_centerw)**2)
        if(distance3>b3radius):
            b3mask[h,w] = 1

b3mask = np.fft.ifftshift(b3mask)
#apply high pass filter 
applyFilter3 = B3_transform*b3mask

#map transformed pixel values
inverseb3 = np.abs(np.fft.ifft2(applyFilter3))
inverseb3 = inverseb3/255.0
result3 = domap(inverseb3,np.min(inverseb3),np.max(inverseb3),0.0,1.0)
img.imsave("B3_output.jpg",result3,cmap='gray')