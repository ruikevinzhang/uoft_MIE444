%provide measurements and movements
%heading = 270; %270 degrees implied down
headingguess = [0,90,180, 270];

% 
% %set1 heading 270
% m_lr = [1,1,1, 0,1,0,0, 1,0,1,1, 0,1,0,1, 1,0,1,1, 0,1,0,0, 0,0,0,0];
% m_rr = [1,0,1, 0,1,1,0, 1,0,1,0, 0,0,1,0, 0,0,0,1, 1,0,0,0, 0,0,1,0];
% m_u  = [3,3,3, 1,1,1,1, 5,5,5,5, 0,0,0,0, 5,5,5,5, 2,2,2,2, 2,2,2,2];
% m_m = ['w','w','w', 'w','w','r','w', 'w','w','w','w', 'w','w','w','w', 'w','w','w','w', 'w','w','r','w', 'w','w','l','w'];

% %set2 heading 90
% m_lr = [0,1,1, 1,0,0,0, 1,0,0,0, 1,0,1,1, 0,1,0,1    ,1,0,1,1, 0,1,0,0, 0,0,0,0];
% m_rr = [1,0,1, 1,0,0,1, 0,0,1,0, 1,0,1,0, 0,0,1,0    ,0,0,0,1, 1,0,0,0, 0,0,1,0];
% m_u  = [3,3,3, 5,5,5,5, 1,1,1,1, 5,5,5,5, 0,0,0,0    ,5,5,5,5, 2,2,2,2, 2,2,2,2];
% m_m = ['w','w','w', 'w','w','w','w', 'w','w','l','w', 'w','w','w','w', 'w','w','w','w', 'w','w','w','w', 'w','w','r','w' ,'w','w','l','w'];
% 


%set3 heading 270
% m_lr = [1,1,1, 0,0,0,1,    1,0,1,1, 0,1,0,0, 0,0,0,0];
% m_rr = [0,0,0, 1,1,1,0,    0,0,0,1, 1,0,0,0, 0,0,1,0];
% m_u  = [3,3,3, 0,0,0,0,    5,5,5,5, 2,2,2,2, 2,2,2,2];
% m_m = ['w','w','w',  'w','w','r','w', 'w','w','w','w',  'w','w','r','w',  'w','w','l','w'];




%set4 heading 270
m_lr = [0,0,0, 1,0,0,0, 0,0,1,1, 1,0,0,1, 0,1,1,1];
m_rr = [1,0,0, 0,1,1,1, 1,1,1,0, 0,0,0,0, 1,1,0,0];
m_u  = [3,3,3, 1,1,1,1, 5,5,5,5, 2,2,2,2, 5,5,5,5];
m_m = ['w','w','w',  'w','w','r','w', 'w','w','w','w',  'w','w','r','w',  'w','w','w','w'];




%Initalization of the worlr
dim1 = 32; dim2 = 16; 
locationindex = reshape(1:dim1*dim2,dim1,dim2)'; 
n = numel(locationindex);
rand('twister',5489);
bw = reshape(randi([0 1],n,1),dim2,dim1); %0 = black, 1 = white

%Make blocks(blockage)
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
figure; imagesc((bw+1).*M); 

%initialize probability
p1 = ones(dim2,dim1)*(1/n); 
p2 = ones(dim2,dim1)*(1/n); 
p3 = ones(dim2,dim1)*(1/n); 
p4 = ones(dim2,dim1)*(1/n); 
pheading = ones(4,1);
confirm_heading =0;

%figure;
%% localizatoin loop


for k = 1:length(m_m),
    if confirm_heading == 0
        headingguess = headchange (headingguess, m_m(k));
        %sensor update
        p1 = sense_2r_u(bw,ultra, M, p1, m_lr(k),m_rr(k),m_u(k),headingguess(1));
        p2 = sense_2r_u(bw,ultra, M, p2, m_lr(k),m_rr(k),m_u(k),headingguess(2));
        p3 = sense_2r_u(bw,ultra, M, p3, m_lr(k),m_rr(k),m_u(k),headingguess(3));
        p4 = sense_2r_u(bw,ultra, M, p4, m_lr(k),m_rr(k),m_u(k),headingguess(4));
        %calculate probability for heading
        %find the max probability in all four directions
        m = 0.2*ones(4,1);
        pmax(1) = max(p1(:)) ;
        pmax(2) = max(p2(:)) ;
        pmax(3) = max(p3(:)) ;
        pmax(4) = max(p4(:)) ;
        %which one is the most probability in four direction and assign it 0.6
        [mh,I]=max(pmax);
        m(I) = 0.6;
        %apply to the probability and normalize it 
        pheading = pheading.*m;
        pheading = pheading./sum(pheading);

        imagesc(p4);
        [row, col] = find(ismember(p4, max(p4(:))))

        location=[ceil(row/4),ceil(col/4)];
        disp(location);
        title(['step: ' num2str(k)]);
        pause(1);

        [p1] = move_only(p1, M, headingguess(1), m_m(k));
        [p2] = move_only(p2, M, headingguess(2), m_m(k));
        [p3] = move_only(p3, M, headingguess(3), m_m(k));
        [p4] = move_only(p4, M, headingguess(4), m_m(k));
        
        if pheading(I) > 0.8
            confirm_heading = 1;
            heading = pheading(I);
            if I == 1
                p = p1;
            elseif I == 2
                p = p2;
            elseif I == 3
                p = p3;
            elseif I == 4
                p = p4;
            end
        end 
        else
            
        end
        
               
        
        
        

    
    

end
