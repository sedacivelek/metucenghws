## Group members
## Seda Civelek 2237147
## Kağan Erdoğan 2098986

import sys
import numpy as np
import matplotlib.image as img
import pywt
import os

def compress(image,waveletType,eps):
    coeff = pywt.wavedec2(image,waveletType)
    coeffa, coeffs = pywt.coeffs_to_array(coeff)
    #sort 
    sortedCoeffa = np.sort(np.abs(coeffa).reshape(-1)) 
    #descending order
    sortedCoeffa = np.flip(sortedCoeffa,0) 
    #define threshold value based on given epsilon
    treshold = int(eps*len(sortedCoeffa)) 
    tVal = sortedCoeffa[treshold]
    #cut details
    cuttedArray = coeffa*(abs(coeffa)>tVal)
    return cuttedArray,coeffs

def decompress(coeffa,coeffs,waveletType):
    coeff = pywt.array_to_coeffs(coeffa,coeffs,output_format='wavedec2')
    #reconstruct image
    nimage = pywt.waverec2( coeff,waveletType)
    return nimage

def main(argv):
    image = img.imread(argv[1])
    height = image.shape[0]
    waveletType = 'haar'
    eps = 0.01
    compressedArray, coeffs = compress(image,waveletType,eps)
    nimage = decompress(compressedArray,coeffs,waveletType)
    img.imsave("THE2-Images/output_"+argv[1],np.clip(nimage,0,255),cmap='gray')
    mse =0
    for i in range(image.shape[0]):
        for j in range(image.shape[1]):
            mse = mse+(image[i][j]-nimage[i][j])*(image[i][j]-nimage[i][j])   
    mse = mse/(image.shape[0]*image.shape[1])
    print(("MSE and Compression Ratio using {} threshold value and {} wavelet family").format(eps,waveletType))  
    print(("MSE: {}").format(mse))
    outputsize = os.path.getsize("output_"+argv[1])
    origsize = os.path.getsize(argv[1])
    print(("Compression ratio: {}").format(origsize/outputsize))
    
if __name__ =="__main__":
    main(sys.argv)
