%provide measurements and movements
heading = [0 90 180 270]; %270 degrees implied down
m_u = [ 3,  3,  3,  1,  1,  1,  1,  5,  5,  5,  5,  0,  0,  0,  0,  5,  5,  5,  5,  2,  2,  2,  2,  2,  2];
m_m = ['w','w','w','d','a','d','w','w','w','w','w','w','w','w','w','w','w','w','w','w','d','w','w','w','a'];

%initalization of the world
dim1 = 32; dim2 = 16; 
locationindex = reshape(1:dim1*dim2,dim1,dim2)'; 
n = numel(locationindex);
rand('twister',5489);
bw = reshape(randi([0 1],n,1),dim2,dim1); %0 = black, 1 = white

%make blocks
M = zeros(size(bw));
Blocks = [2, 3; 3, 2; 4, 3; 5, 1; 5, 3; 7, 1; 7, 3; 7, 4;];
for xx = 1:size(Blocks,1),
	x = Blocks(xx,1); y = Blocks(xx,2);
	M(1+(y-1)*4:(y-1)*4+4, 1+(x-1)*4:(x-1)*4+4) = 1;
end
M = [ones(dim2,1) M ones(dim2,1)];
M = [ones(1, dim1+2); M; ones(1, dim1+2)];

%generate ultrasonic world
ultra = zeros(size(bw));
for sec_row = 1:4:dim2,
    for sec_col = 1:4:dim1,
        segRow = M(sec_row+2, sec_col:sec_col+5);
        segCol = M(sec_row:sec_row+5, sec_col+2);
        val = sum(segRow)+sum(segCol);
        if val == 2 && sum(segRow)~=1,
            val = 5;
        end
        ultra(sec_row:sec_row+3, sec_col:sec_col+3) = val;
    end
end

%create mask for blocks
M = abs(M-1);
M = M(2:end-1, 2:end-1);
figure; imagesc((bw+1).*M); colormap(gray);

%initialize probability
p1 = ones(dim2,dim1)*(1/n); 
p2 = ones(dim2,dim1)*(1/n); 
p3 = ones(dim2,dim1)*(1/n); 
p4 = ones(dim2,dim1)*(1/n); 

figure;
%localizatoin loop

for k = 1:length(m_m),

    %sensor update
    p1 = sense_u(ultra, M, p1, m_u(k));
    p2 = sense_u(ultra, M, p2, m_u(k));
    p3 = sense_u(ultra, M, p3, m_u(k));
    p4 = sense_u(ultra, M, p4, m_u(k));

    subplot(2,2,1);
    imagesc(p1);
    title(heading(1));
    subplot(2,2,2);
    imagesc(p2);
    title(heading(2));
    subplot(2,2,3);    
    imagesc(p3);
    title(heading(3));
    subplot(2,2,4);
    imagesc(p4);
    title(heading(4));
    %title(['step: ' num2str(k)]);
    pause;

    %movement update
    [p1, heading(1)] = move(p1, M, heading(1), m_m(k));
    [p2, heading(2)] = move(p2, M, heading(2), m_m(k));
    [p3, heading(3)] = move(p3, M, heading(3), m_m(k));
    [p4, heading(4)] = move(p4, M, heading(4), m_m(k));

end
