%Seda Civelek-2237147
function q3generic(path)
[filepath,name,ext] = fileparts(path);
Im = imread(path);
I = rgb2gray(Im);
BW = edge(I,'Canny');
SE = strel('line',3,45);
BW = bwareaopen(BW,200);
BW1 = imclose(BW,SE);
imwrite(BW1,"q3Output/step1_"+name+".png");
[H,T,R] = hough(BW1);
imshow(H,[],'XData',T,'YData',R,...
             'InitialMagnification','fit');
 xlabel('\theta'), ylabel('\rho');
 axis on, axis normal, hold on;
 P  = houghpeaks(H,100,'threshold',ceil(0.1*max(H(:))));
 x = T(P(:,2)); y = R(P(:,1));
 plot(x,y,'s','color','white');
 lines = houghlines(BW,T,R,P,'FillGap',5,'MinLength',20);
 detected = figure('Visible', 'off');
 imshow(Im), hold on
 max_len = 0;
 for k = 1:length(lines)
    xy = [lines(k).point1; lines(k).point2];
    plot(xy(:,1),xy(:,2),'LineWidth',2,'Color','green');
    len = norm(lines(k).point1 - lines(k).point2);
    if ( len > max_len)
       max_len = len;
       xy_long = xy;
    end
 end
 plot(xy_long(:,1),xy_long(:,2),'LineWidth',2,'Color','green');
 print(detected,'-dpng','-r320', "q3Output/final_"+name+".png");