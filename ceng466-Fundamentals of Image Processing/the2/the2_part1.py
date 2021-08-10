## Group members
## Seda Civelek 2237147
## Kağan Erdoğan 2098986

import numpy as np
import matplotlib.pyplot as plt
from scipy import fftpack



im1 = plt.imread('A1.png').astype(float)

im2 = plt.imread('A2.png').astype(float)

im3 = plt.imread('A3.png').astype(float)


#-----------  Denoising A1.png ------------------

im1_fft = fftpack.fft2(im1)
shift_im1 = fftpack.fftshift(im1_fft)




im1x , im1y = im1.shape

for i in range(im1x):
    for c in range(im1y):
        if (c >= 365 and c <= 365) and (i <= 237 or i>= 252)  :
            shift_im1[i][c] = 0






decenterim1 = fftpack.ifftshift(shift_im1)

finalim1 = fftpack.ifft2(decenterim1).real
plt.imsave('A1_denoised.png',finalim1,cmap='gray')



# --------------- Denoising A3.png -------------------

im3b , im3g , im3r = im3[:,:,0],im3[:,:,1],im3[:,:,2]



im3b_fft = fftpack.fft2(im3b)
shift3b_fft = fftpack.fftshift(im3b_fft)

im3g_fft = fftpack.fft2(im3g)
shift3g_fft = fftpack.fftshift(im3g_fft)

im3r_fft = fftpack.fft2(im3r)
shift3r_fft = fftpack.fftshift(im3r_fft)


for x in range(1141):
    for y in range(1857):
        if (y >= 880 and y <= 883) or (y >= 972 and y <= 976) or (x >= 540 and x <= 543) or (x >= 598 and x <= 601)  :
            shift3b_fft[x][y] = 0
            shift3g_fft[x][y] = 0
            shift3r_fft[x][y] = 0


decenter3b = fftpack.ifftshift(shift3b_fft)
decenter3g = fftpack.ifftshift(shift3g_fft)
decenter3r = fftpack.ifftshift(shift3r_fft)

final3b = fftpack.ifft2(decenter3b).real
final3g = fftpack.ifft2(decenter3g).real
final3r = fftpack.ifft2(decenter3r).real

rgb = np.dstack((final3b,final3g,final3r))

def normalize(x):

    return np.array((x - np.min(x)) / (np.max(x) - np.min(x)))



plt.imsave('A3_denoised.png',normalize(rgb))




# --------------------- Denoising A2.png --------------------------


im_fft = fftpack.fft2(im2)
shift_fft = fftpack.fftshift(im_fft)



rows, cols = im2.shape
crow, ccol = int(rows / 2), int(cols / 2)
filter = np.ones((rows, cols), dtype=float)
r_out = 85
r_in = 63

rout2 = 320
rin2= 278
center = [crow, ccol]
x, y = np.ogrid[:rows, :cols]
mask_area = np.logical_and(((x - center[0]) ** 2 + (y - center[1]) ** 2 >= r_in ** 2),
                           ((x - center[0]) ** 2 + (y - center[1]) ** 2 <= r_out ** 2))
filter[mask_area] = 0

mask_area2 = np.logical_and(((x - center[0]) ** 2 + (y - center[1]) ** 2 >= rin2 ** 2),
                           ((x - center[0]) ** 2 + (y - center[1]) ** 2 <= rout2 ** 2))
filter[mask_area2] = 0

filtered_image = shift_fft * filter

dec_filtered_image = fftpack.ifftshift(filtered_image)

im_new = fftpack.ifft2(dec_filtered_image).real

plt.imsave('A2_denoised.png',im_new,cmap='gray')
