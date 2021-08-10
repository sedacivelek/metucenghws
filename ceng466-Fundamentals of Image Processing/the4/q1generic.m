%Seda Civelek-2237147
function q1generic(path,inverse)
[filepath,name,ext] = fileparts(path);
A = imread(path);
[L,N] = superpixels(A,500);
outputImage = zeros(size(A),'like',A);
idx = label2idx(L);
numRows = size(A,1);
numCols = size(A,2);
for labelVal = 1:N
    redIdx = idx{labelVal};
    greenIdx = idx{labelVal}+numRows*numCols;
    blueIdx = idx{labelVal}+2*numRows*numCols;
    outputImage(redIdx) = mean(A(redIdx));
    outputImage(greenIdx) = mean(A(greenIdx));
    outputImage(blueIdx) = mean(A(blueIdx));
end    
outputImage = rgb2gray(outputImage);
if inverse ==0
    BW =imbinarize(outputImage,0.5);
end
if inverse == 1
    BW =~imbinarize(outputImage,0.5);
end
step1 = fullfile('q1output',"step1_image"+name+ext);
imwrite(BW,step1);
info = regionprops(BW,'Boundingbox') ;
BW = bwmorph(BW,'skel',Inf);
fr = figure('Visible', 'off');
imshow(BW)
hold on
for k = 1 : length(info)
     BB = info(k).BoundingBox;
     rectangle('Position', [BB(1),BB(2),BB(3),BB(4)],'EdgeColor','r','LineWidth',0.5) ;
end
% frm = getframe(fr);
% set(frm, 'Position', [0 0 size(BW,2), size(BW,1)])
step2_3 = fullfile('q1output',"step2_3_image"+name+ext);
print(fr,'-dpng','-r320', step2_3);

