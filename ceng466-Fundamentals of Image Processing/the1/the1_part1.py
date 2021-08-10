## Group members
## Seda Civelek 2237147
## Kağan Erdoğan 2098986
import sys
import matplotlib.image as img
import numpy as np
import matplotlib.pyplot as plt

def find_match(val, target_arr):
    i = (np.abs(target_arr-val)).argmin()
    return i

def main(argv):
    # Read original and reference images A1=original A2=reference
    a1 = img.imread(argv[1])
    a1_ref = img.imread(argv[2])
    name = argv[1][:2]
    name2 = argv[2][:2]
    a1height = a1.shape[0]
    a1width = a1.shape[1]
    a1_refheight = a1_ref.shape[0]
    a1_refwidth = a1_ref.shape[1]
    #A1 histogram for 3 color channels rgb
    a1_channel_r = np.zeros(256)
    a1_channel_g = np.zeros(256)
    a1_channel_b = np.zeros(256)

    for h in np.array(a1):
        for w in h :
            val_r = w[0]
            val_g = w[1]
            val_b = w[2]
            
            a1_channel_r[val_r] += 1
            a1_channel_g[val_g] += 1
            a1_channel_b[val_b] += 1
    
    #Cumulative histogram of A1
    for i in range(1,256):
        a1_channel_r[i]=a1_channel_r[i]+a1_channel_r[i-1]
        a1_channel_g[i]=a1_channel_g[i]+a1_channel_g[i-1]
        a1_channel_b[i]=a1_channel_b[i]+a1_channel_b[i-1]

    #Equalized cumulative histogram of A1 
    a1_output_r = np.zeros(256)
    a1_output_g = np.zeros(256)
    a1_output_b = np.zeros(256)
    for y in range(a1height) :
        for x in range(a1width) :
            a1_output_r[a1[y,x,0]] = round(255*(1.0/(a1width*a1height))*a1_channel_r[a1[y,x,0]])
            a1_output_g[a1[y,x,1]] = round(255*(1.0/(a1width*a1height))*a1_channel_g[a1[y,x,1]])
            a1_output_b[a1[y,x,2]] = round(255*(1.0/(a1width*a1height))*a1_channel_b[a1[y,x,2]])

    #A1_ref histogram for 3 color channels rgb
    a1_ref_channel_r = np.zeros(256)
    a1_ref_channel_g = np.zeros(256)
    a1_ref_channel_b = np.zeros(256)
    for h in np.array(a1_ref):
        for w in h:
            val_ref_r = w[0]
            val_ref_g = w[1]
            val_ref_b = w[2]
            a1_ref_channel_r[val_ref_r] += 1
            a1_ref_channel_g[val_ref_g] += 1
            a1_ref_channel_b[val_ref_b] += 1

    
    #Cumulative histogram of A1_ref
    for i in range(1,256):
        a1_ref_channel_r[i]=a1_ref_channel_r[i]+a1_ref_channel_r[i-1]
        a1_ref_channel_g[i]=a1_ref_channel_g[i]+a1_ref_channel_g[i-1]
        a1_ref_channel_b[i]=a1_ref_channel_b[i]+a1_ref_channel_b[i-1]

    #Equalized cumulative histogram of A1_ref
    a1ref_output_r = np.zeros(256)
    a1ref_output_g = np.zeros(256)
    a1ref_output_b = np.zeros(256)
    for y in range(a1_refheight) :
        for x in range(a1_refwidth) :
            a1ref_output_r[a1_ref[y,x,0]] = round(255*(1.0/(a1_refwidth*a1_refheight))*a1_ref_channel_r[a1_ref[y,x,0]])
            a1ref_output_g[a1_ref[y,x,1]] = round(255*(1.0/(a1_refwidth*a1_refheight))*a1_ref_channel_g[a1_ref[y,x,1]])
            a1ref_output_b[a1_ref[y,x,2]] = round(255*(1.0/(a1_refwidth*a1_refheight))*a1_ref_channel_b[a1_ref[y,x,2]])

    #Histogram matching
    t_hist_r = np.zeros(256)
    t_hist_g = np.zeros(256)
    t_hist_b = np.zeros(256)
    for i in range(len(a1_output_r)):
        ir =find_match(val=a1_output_r[i],target_arr=a1ref_output_r)
        t_hist_r[i] = ir

    for i in range(len(a1_output_g)):
        ig = find_match(val=a1_output_g[i],target_arr=a1ref_output_g)
        t_hist_g[i] = ig
    for i in range(len(a1_output_b)):
        ib = find_match(val=a1_output_b[i],target_arr=a1ref_output_b)
        t_hist_b[i] = ib

    #Create transformed output
    matched_img = np.zeros_like(a1)
    for y in range(a1height):
        for x in range(a1width):
            valr = int(a1[y,x,0])
            valg = int(a1[y,x,1])
            valb = int(a1[y,x,2])
            matched_img[y,x,0] = t_hist_r[valr]
            matched_img[y,x,1] = t_hist_g[valg]
            matched_img[y,x,2] = t_hist_b[valb]
    
    a1_match_r = np.zeros(256)
    a1_match_g = np.zeros(256)
    a1_match_b = np.zeros(256)
    for h in np.array(matched_img):
        for w in h :
            val_r = w[0]
            val_g = w[1]
            val_b = w[2]
            
            a1_match_r[val_r] += 1
            a1_match_g[val_g] += 1
            a1_match_b[val_b] += 1
    #Print all rgb histogram matching plots and transformed output
    img.imsave(name+"_histmatch_output.png",matched_img)
    fig, axs = plt.subplots(3,sharex=True,sharey=True)
    fig.suptitle("Histogram matching of "+ argv[1]+" when reference is "+argv[2])
    axs[0].plot(a1_match_r, color='#FF0000')
    axs[0].bar(np.arange(len(a1_match_r)), a1_match_r, color='#FF0000')
    axs[0].set_ylabel('Number of Pixels')
    axs[0].set_xlabel('Pixel Value')
    axs[1].plot(a1_match_g, color='#00FF00')
    axs[1].bar(np.arange(len(a1_match_g)), a1_match_g, color='#00FF00')
    axs[1].set_ylabel('Number of Pixels')
    axs[1].set_xlabel('Pixel Value')
    axs[2].plot(a1_match_b, color='#0000FF')
    axs[2].bar(np.arange(len(a1_match_b)), a1_match_b, color='#0000FF')
    axs[2].set_ylabel('Number of Pixels')
    axs[2].set_xlabel('Pixel Value')
    fig.savefig(name+"_histmatch")
    
if __name__ =="__main__":
    main(sys.argv)