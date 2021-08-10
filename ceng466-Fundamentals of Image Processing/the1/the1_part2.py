## Group members
## Seda Civelek 2237147
## Kağan Erdoğan 2098986
import sys
import matplotlib.image as img
import numpy as np
import matplotlib.pyplot as plt

def convolution(image,filt):
    height = image.shape[0]
    width = image.shape[1]
    filth = filt.shape[0]
    filtw = filt.shape[1]
    newimage  = np.zeros((height+filth-1,width+filtw-1,3))
    conimage = np.zeros_like(image)
    index = int((filth-1)/2)
    for h in range(image.shape[0]):
        for w in range(image.shape[1]):
            newimage[h+index,w+index,0] = image[h,w,0]
            newimage[h+index,w+index,1] = image[h,w,1]
            newimage[h+index,w+index,2] = image[h,w,2]
    for y in range(conimage.shape[0]):
        for x in range(conimage.shape[1]):
            imsize = np.size(newimage[y:y+filth,x:x+filtw,0])
            fsize = np.size(filt)
            if imsize == fsize:
                conimage[y,x,0] = np.sum(filt*newimage[y:y+filth,x:x+filtw,0])
                conimage[y,x,1] = np.sum(filt*newimage[y:y+filth,x:x+filtw,1])
                conimage[y,x,2] = np.sum(filt*newimage[y:y+filth,x:x+filtw,2])
    return conimage

def gaussianSmoothing(k,sigma):
    k = int(k)
    x,y = np.mgrid[-k:k+1,-k:k+1]
    gaussian = (1/((2.0 * np.pi)*sigma**2))*np.exp((-(x**2+y**2)/(2*sigma**2)))
    return gaussian
def sobelFilter(image):
    kh = np.array([[1, 2, 1], [0, 0, 0], [-1, -2, -1]])
    kv = np.array([[1, 2, 1], [0, 0, 0], [-1, -2, -1]])
    gh = convolution(image,kh)
    gv = convolution(image,kv)
    magn = np.hypot(gh,gv)
    magn = magn/magn.max()*255
    theta = np.rad2deg(np.arctan2(gh,gv))
    theta += 180
    return (magn,theta,gh,gv)

def nonMaxSuppression(image,direction):
    height = image.shape[0]
    width = image.shape[1]
    newimage = np.zeros((height,width,3))
    angle= 180
    for y in range(1,height-1):
        for x in range(1,width-1):
            q0 = 255
            q1 = 255
            q2 = 255
            r0 = 255
            r1 = 255
            r2 = 255
            if(0<=direction[y,x,0]<angle/8) or (15*angle/8<=direction[y,x,0]<=2*angle):
                q0=image[y,x-1,0]
                r0=image[y,x+1,0]
            elif (angle/8 <= direction[y,x,0] < 3*angle/8) or (9*angle/8<=direction[y,x,0]<11*angle/8): 
                q0 =image[y+1,x-1,0]
                r0 =image[y-1,x+1,0]
            elif (3*angle/8 <= direction[y,x,0] < 5*angle/8) or (11*angle/8<=direction[y,x,0]<13*angle/8):
                q0 = image[y-1,x,0]
                r0 = image[y+1,x,0]
            else:
                q0 = image[y-1,x-1,0]
                r0 = image[y+1,x+1,0]
            if (image[y,x,0]>=q0) and (image[y,x,0]>=r0):
                newimage[y,x,0]=image[y,x,0]
            else:
                newimage[y,x,0]=0
            
            if(0<=direction[y,x,1]<angle/8) or (15*angle/8<=direction[y,x,1]<=2*angle):
                q1=image[y,x-1,1]
                r1=image[y,x+1,1]
            elif (angle/8 <= direction[y,x,1] < 3*angle/8) or (9*angle/8<=direction[y,x,1]<11*angle/8): 
                q1 =image[y+1,x-1,1]
                r1 =image[y-1,x+1,1]
            elif (3*angle/8 <= direction[y,x,1] < 5*angle/8) or (11*angle/8<=direction[y,x,1]<13*angle/8):
                q1 = image[y-1,x,1]
                r1 = image[y+1,x,1]
            else:
                q1 = image[y-1,x-1,1]
                r1 = image[y+1,x+1,1]
            if (image[y,x,0]>=q1) and (image[y,x,0]>=r1):
                newimage[y,x,1]=image[y,x,1]
            else:
                newimage[y,x,1]=0

            if(0<=direction[y,x,2]<angle/8) or (15*angle/8<=direction[y,x,2]<=2*angle):
                q2=image[y,x-1,2]
                r2=image[y,x+1,2]
            elif (angle/8 <= direction[y,x,2] < 3*angle/8) or (9*angle/8<=direction[y,x,2]<11*angle/8): 
                q2 =image[y+1,x-1,2]
                r2 =image[y-1,x+1,2]
            elif (3*angle/8 <= direction[y,x,2] < 5*angle/8) or (11*angle/8<=direction[y,x,2]<13*angle/8):
                q2 = image[y-1,x,2]
                r2 = image[y+1,x,2]
            else:
                q2 = image[y-1,x-1,2]
                r2 = image[y+1,x+1,2]
            if (image[y,x,2]>=q2) and (image[y,x,2]>=r2):
                newimage[y,x,2]=image[y,x,2]
            else:
                newimage[y,x,2]=0
                
    return newimage
def doubleThreshold(image,low,high,weak):
    newimg = np.zeros((image.shape[0],image.shape[1],3))
    strong = 255
    newimg = np.where(image >= high, strong , newimg)
    newimg = np.where((image<=high)&(image>=low),weak,newimg)
    return newimg
def hysteresis(img):
    height = img.shape[0]
    width = img.shape[1]
    for y in range(1,height-1):
        for x in range(1,width-1):
            if(img[y,x,0]==25):
                if(img[y+1,x-1,0]==255)or (img[y+1,x,0]==255) or (img[y+1,x+1,0]==255)or (img[y,x-1,0]==255) or (img[y,x+1,0]==255) or (img[y-1,x-1,0]==255) or (img[y-1,x,0]==255) or (img[y-1,x+1,0]==255):
                    img[y,x,0] = 255
                else:
                    img[y,x,0] = 0
            if(img[y,x,1]==25):
                if(img[y+1,x-1,1]==255)or (img[y+1,x,1]==255) or (img[y+1,x+1,1]==255)or (img[y,x-1,1]==255) or (img[y,x+1,1]==255) or (img[y-1,x-1,1]==255) or (img[y-1,x,1]==255) or (img[y-1,x+1,1]==255):
                    img[y,x,1] = 255
                else:
                    img[y,x,1] = 0
            if(img[y,x,0]==25):
                if(img[y+1,x-1,2]==255)or (img[y+1,x,2]==255) or (img[y+1,x+1,2]==255)or (img[y,x-1,2]==255) or (img[y,x+1,2]==255) or (img[y-1,x-1,2]==255) or (img[y-1,x,2]==255) or (img[y-1,x+1,2]==255):
                    img[y,x,2] = 255
                else:
                    img[y,x,2] = 0
    return img



def CannyEdge(image):
    g = gaussianSmoothing(2,1) #size = 2k+1 first parameter is k, second is sigma
    newimage =convolution(image,g)
    sobelmag, sobeltheta, gh,gv= sobelFilter(newimage)
    nonmax = nonMaxSuppression(sobelmag,sobeltheta)
    tresh = doubleThreshold(nonmax,5,20,25)
    last = hysteresis(tresh)
    last= last.astype(np.uint8)
    return last
    
def main(argv):
    image = img.imread(argv[1])
    name = argv[1][:2]
    laplacian = np.array([[-1, -1, -1], [-1, 8, -1], [-1, -1, -1]])
    cannyEdge = CannyEdge(image) 
    img.imsave(name+"cannyedge.png",cannyEdge)
if __name__ =="__main__":
    main(sys.argv)