%members
%Seda Civelek-2237147
%Kağan Erdoğan-2098986

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%A1 operations%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%read file, convert to gray scale
A1 = imread("THE3-Images/A1.png");
A1 = rgb2gray(A1);
%retrieve height and width
a1h = size(A1,1);
a1w = size(A1,2);

%using threshold convert grayscale image to binary image
%threshold value determined by object's pixel color
a1_binary = zeros(a1h,a1w,'logical');
for h=1:a1h
    for w=1:a1w
        if(A1(h,w)>90)
            a1_binary(h,w) = 0;
        else
            a1_binary(h,w) = 1;
        end
    end
end
SE = ones(5,5,'logical');
%first dilate image
a1_binary = imdilate(a1_binary,SE);
%then erode image
a1_binary = imerode(a1_binary,SE);
%count number of objects
[res1,n1] = bwlabel(a1_binary);
imwrite(res1,'part1_A1.png');
fprintf("The number of flying jets in image A1 is %d\n",n1);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%A2 operations%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%read file, convert to gray scale
A2 = imread("THE3-Images/A2.png");
A2 = rgb2gray(A2);
%retrieve height and width
a2h = size(A2,1);
a2w = size(A2,2);
%using threshold convert grayscale image to binary image
%since image has darker colors in lower(trees) and lighter colors in
%upper(sky), threshold values determined differently for upper and lower
%sides of the image
a2_binary = zeros(a2h,a2w,'logical');
for h=1:round(a2h/2)
    for w=1:a2w
        if(A2(h,w)>110 || A2(h,w)<60)
            a2_binary(h,w) = 0;
        else
            a2_binary(h,w) = 1;
        end
    end
end
for h=round(a2h/2)+1:a2h
    for w=1:a2w
        if(A2(h,w)>100 || A2(h,w)<70)
            a2_binary(h,w) = 0;
        else
            a2_binary(h,w) = 1;
        end
    end
end
%structure element for erosion
SE = ones(3,3,'logical');
%structure element for dilation
SE2 = ones(16,16,'logical');
%the line between sky and trees is still visible, get rid of using erosion
a2_binary = imerode(a2_binary,SE);
%connect separate parts of jets using dilation
a2_binary = imdilate(a2_binary,SE2);
%there are still holes in jets, fill them using imfill
a2_binary = imfill(a2_binary,'holes');
%some pixels in trees couldnt thresholded, since they are small components
%get rid of them using bwareopen
a2_binary = bwareaopen(a2_binary,1500);
%count objects
[res2,n2] = bwlabel(a2_binary);
imwrite(res2,'part1_A2.png');
fprintf("The number of flying jets in image A2 is %d\n",n2);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%A3 operations%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%read file, convert to gray scale
A3 = imread("THE3-Images/A3.png");
A3 = rgb2gray(A3);
%retrieve height and width
a3h = size(A3,1);
a3w = size(A3,2);
%using threshold convert grayscale image to binary image
%threshold value determined by object's and smoke's pixel color
a3_binary = zeros(a3h,a3w,'logical');
for h=1:a3h
    for w=1:a3w
        if(A3(h,w)>65)
            a3_binary(h,w) = 0;
        else
            a3_binary(h,w) = 1;
        end
    end
end
SE = ones(3,3,'logical');
%to connect separate parts and fill black parts of the jets use imclose
a3_binary = imclose(a3_binary,SE);
%there are still noise around jets, to get rid of them use opening
a3_binary = bwareaopen(a3_binary,10);
%count the number of objects
[res3,n3] = bwlabel(a3_binary);
imwrite(res3,'part1_A3.png');
fprintf("The number of flying jets in image A3 is %d\n",n3);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%A4 operations%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%read file, convert to gray scale
A4 = imread("THE3-Images/A4.png");
A4 = rgb2gray(A4);
%retrieve height and width
a4h = size(A4,1);
a4w = size(A4,2);
a4_binary = zeros(a4h,a4w,'logical');
for h=1:a4h
    for w=1:a4w
        if(A4(h,w)>60)
            a4_binary(h,w) = 0;
        else
            a4_binary(h,w) = 1;
        end
    end
end
SE = ones(7,7,'logical');

SE1 = [ 1 1 1 1 1 1 1 1 1 1 1 ];
a4_binary = imerode(a4_binary,SE1); 
a4_binary = imclose(a4_binary,SE);
[res4,n4] = bwlabel(a4_binary);
imwrite(res4,'part1_A4.png');
fprintf("The number of flying jets in image A4 is %d\n",n4);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%A5 operations%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

A5 = imread("THE3-Images/A5.png");
A5 = A5(:,:,3);
a5h = size(A5,1);
a5w = size(A5,2);
a5_binary = zeros(a5h,a5w,'logical');
for h=1:a5h
    for w=1:a5w
        if(A5(h,w)>155 && A5(h,w)<200)
            a5_binary(h,w) = 0;
        else
            a5_binary(h,w) = 1;
        end
    end
end
%get rid of noise on the right side using bwareaopen
a5_binary = bwareaopen(a5_binary,10);
%connect separate parts of jets
SE = ones(11,11,'logical');
a5_binary = imclose(a5_binary,SE);
%fill holes in the jet
a5_binary = imfill(a5_binary,'holes');
%remove letters on the lower right conner using bwareaopen
a5_binary = bwareaopen(a5_binary, 4200);
[res5,n5] = bwlabel(a5_binary);
imwrite(res5,'part1_A5.png');
fprintf("The number of flying jets in image A5 is %d\n",n5);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%A6 operations%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

A6 = imread("THE3-Images/A6.png");
A6 = A6(:,:,3);
a6h = size(A6,1);
a6w = size(A6,2);
a6_binary = zeros(a6h,a6w,'logical');
for h=1:a6h
    for w=1:a6w
        if(A6(h,w)>55 || (w<a6w/2 && h>a6h/2 && h<(a6h/2+120)))
            a6_binary(h,w) = 0;
        else
            a6_binary(h,w) = 1;
        end
    end
end
%to get rid of noise on the right side
a6_binary = bwareaopen(a6_binary,30);
SE = ones(5,5,'logical');
%connect the separate parts of jets
a6_binary = imclose(a6_binary,SE);
%to fill holes in jets
a6_binary = imfill(a6_binary,'holes');
[res6,n6] = bwlabel(a6_binary);
imwrite(res6,'part1_A6.png');
fprintf("The number of flying jets in image A6 is %d\n",n6);



