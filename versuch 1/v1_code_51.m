[xnT,fs] = audioread('Menu.wav');
t=1/fs;
nxl=0;
nxr=length(xnT)-1;
ix=length(xnT);
sound(xnT,fs);
hnT=[0.15 0.0 0.15 -0.15 -0.3 -0.45 1.5 -0.45 -0.3 -0.15 0.15 0.0 0.15] 
nhl=0;
nhr=12;
ih=length(hnT);
ix=length(xnT);
iy=ix+ih-1;
nyr=nxr+nhr;
nyl=nxl+nhl;
tx=nxl*t:t:nxr*t;
th=nhl*t:t:nhr*t;
ty=nyl*t:t:nyr*t;
B=reshape(xnT,[1,ix]);
iy=ix+ih-1;
y= zeros(1,iy);
for i = 1:ix
    for k = 1:ih
      y(i+k-1) = y(i+k-1) + hnT(k)*B(i);
    end
end