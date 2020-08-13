%Initalization of the world
% clc;
% clear;
o=instrfind;
fclose(o);
% bt = Bluetooth('HC-06', 1);
% fopen(bt);

% bt=serial('COM3');
% bt.Baudrate = 9600;% Establish connection
fopen(bt);

dim1 = 32; dim2 = 16; 
locationindex = reshape(1:dim1*dim2,dim1,dim2)'; 
n = numel(locationindex);
rand('twister',5489);
bw = reshape(randi([0 1],n,1),dim2,dim1); %0 = blaccounter, 1 = white

%Macountere bloccounters(bloccounterage)
M = zeros(size(bw));
Bloccounters = [2, 3; 3, 2; 4, 3; 5, 1; 5, 3; 7, 1; 7, 3; 7, 4;];
for xx = 1:size(Bloccounters,1),
	x = Bloccounters(xx,1); y = Bloccounters(xx,2);
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
%pathfinding map 
nodes = [1 8 4; 2 8 3; 3 8 2; 4 8 1; 5 7 3; 6 6 4; 7 6 3; 8 6 2; 9 6 1; 10 5 3; 11 5 1; 12 4 4; 13 4 3; 14 4 1; 15 3 4; 16 3 2; 17 3 1; 18 2 4; 19 2 3; 20 2 1; 21 1 4; 22 1 3; 23 1 2; 24 1 1];
segments = [1 1 2; 1 2 3; 1 2 5; 1 3 4; 1 5 7; 1 7 6; 1 7 8; 1 7 10; 1 8 9; 1 9 11; 1 10 13; 1 11 14; 1 12 13; 1 12 15; 1 14 17; 1 15 18; 1 16 17; 1 17 20; 1 18 19; 1 18 21; 1 19 22; 1 20 24; 1 21 22; 1 22 23; 1 23 24];
map = [21 18 15 12 0 6 0 1; 22 19 0 13 10 7 5 2; 23 0 16 0 0 8 0 3; 24 20 17 14 11 9 0 4];
map_big=[21,21,21,21,18,18,18,18,15,15,15,15,12,12,12,12,0,0,0,0,6,6,6,6,0,0,0,0,1,1,1,1;21,21,21,21,18,18,18,18,15,15,15,15,12,12,12,12,0,0,0,0,6,6,6,6,0,0,0,0,1,1,1,1;21,21,21,21,18,18,18,18,15,15,15,15,12,12,12,12,0,0,0,0,6,6,6,6,0,0,0,0,1,1,1,1;21,21,21,21,18,18,18,18,15,15,15,15,12,12,12,12,0,0,0,0,6,6,6,6,0,0,0,0,1,1,1,1;22,22,22,22,19,19,19,19,0,0,0,0,13,13,13,13,10,10,10,10,7,7,7,7,5,5,5,5,2,2,2,2;22,22,22,22,19,19,19,19,0,0,0,0,13,13,13,13,10,10,10,10,7,7,7,7,5,5,5,5,2,2,2,2;22,22,22,22,19,19,19,19,0,0,0,0,13,13,13,13,10,10,10,10,7,7,7,7,5,5,5,5,2,2,2,2;22,22,22,22,19,19,19,19,0,0,0,0,13,13,13,13,10,10,10,10,7,7,7,7,5,5,5,5,2,2,2,2;23,23,23,23,0,0,0,0,16,16,16,16,0,0,0,0,0,0,0,0,8,8,8,8,0,0,0,0,3,3,3,3;23,23,23,23,0,0,0,0,16,16,16,16,0,0,0,0,0,0,0,0,8,8,8,8,0,0,0,0,3,3,3,3;23,23,23,23,0,0,0,0,16,16,16,16,0,0,0,0,0,0,0,0,8,8,8,8,0,0,0,0,3,3,3,3;23,23,23,23,0,0,0,0,16,16,16,16,0,0,0,0,0,0,0,0,8,8,8,8,0,0,0,0,3,3,3,3;24,24,24,24,20,20,20,20,17,17,17,17,14,14,14,14,11,11,11,11,9,9,9,9,0,0,0,0,4,4,4,4;24,24,24,24,20,20,20,20,17,17,17,17,14,14,14,14,11,11,11,11,9,9,9,9,0,0,0,0,4,4,4,4;24,24,24,24,20,20,20,20,17,17,17,17,14,14,14,14,11,11,11,11,9,9,9,9,0,0,0,0,4,4,4,4;24,24,24,24,20,20,20,20,17,17,17,17,14,14,14,14,11,11,11,11,9,9,9,9,0,0,0,0,4,4,4,4];

%create mascounter for bloccounters
M = abs(M-1);
M = M(2:end-1, 2:end-1);

headingguess = [0,90,180, 270];





p1 = ones(dim2,dim1)*(1/n); 
p2 = ones(dim2,dim1)*(1/n); 
p3 = ones(dim2,dim1)*(1/n); 
p4 = ones(dim2,dim1)*(1/n); 
pheading = ones(4,1);

%testing case 

p = ones(dim2,dim1)*(1/n); 



%% Ardunio interface

heading = 270;
path_final = map(1,8);
%path_final = map(1,6);

% instrhwinfo('Bluetooth','HC-06');
% bt = Bluetooth('HC-06', 1);
%fopen(bt);
counter=1;
size = 10;
confirm_heading = 1;
confirm_unloading = 0;


confirm_local=0;
confirm_loading = 0;
move_counter =0;
local_counter=0;
prelocation = [0,0];
location = [0,0];
heading_reset_counter = 0;
loc_diff =0;
max_p=0;
while true   
    ul_count = 0;
    %extract data from ardunio and store in sensordata
    sample = fscanf(bt);
    disp (sample);
    %check for any input
    if sample ~= ""
%% processing data from ardunio
        ind = find(sample == ',');  
        %only process the data with 10 numbers sure that the 
        if length(ind)+1 == 10
            %takes in with movement 1 = forward , 2 = right, 3 = left, 4  =
            %adjust forward, 6 = uturn, 9 = light(loc)/ idle, 8 =
            %light(unload), 7 = start unloading path find , 5 = idle move
            if (str2double(sample(ind(10-1):end))== 1 || str2double(sample(ind(10-1):end))== 2 || str2double(sample(ind(10-1):end))== 3|| str2double(sample(ind(10-1):end))== 4|| str2double(sample(ind(10-1):end))== 9) || str2double(sample(ind(10-1):end))== 6||  str2double(sample(ind(10-1):end))== 8||  str2double(sample(ind(10-1):end))== 7 || str2double(sample(ind(10-1):end))== 5
                %loop through the 10 number and extrat data
                for i=1:size
                    if i==1
                        sensordata(counter,i) = str2double(sample(1:(ind(i)-1)));                
                    elseif i==size
                        sensordata(counter,i) = str2double(sample(ind(i-1):end)); 
                    else
                        sensordata(counter,i) = str2double(sample(ind(i-1):(ind(i)-1)));
                    end
                end
                %count Ul reading and process UL data ( one direct each)
                for i = 3:9
                    if (i ==6 || i ==8 || i==9)
                        if sensordata(counter,i)<25
                            ul_count = ul_count + 1;
                        end
                    elseif (i == 3 && sensordata(counter,i)<20)
                        ul_count = ul_count + 1;
                    end
                end 
                %two Ul reading case for 2 and 5 (oposite vs adj)
                if ul_count ==2
                    if (sensordata(counter,3) > 20 && sensordata(counter,9) > 25)
                        ul_count =5;
                    elseif(sensordata(counter,6) >25 && sensordata(counter,8) >25)
                        ul_count =5;      
                    end
                end

                %process move data
                %forward =1, r =2, k =3 w =4 d= 5,a=6 , local light = 9
                switch sensordata(counter,10)
                   case 1
                      data_move(counter)= 'f';
                   case 2
                      data_move(counter)= 'r';
                   case 3
                      data_move(counter)= 'l';
                   case 4
                      data_move(counter)= 'w';
                   case 5
                      data_move(counter)= 'd';
                   case 6
                      data_move(counter)= 'a';
                   case 7
                      data_move(counter)= 'q';                      
                   case 9
                      data_move(counter)= 'g';
                   otherwise
                      data_move(counter)= '0';
                end
                %store data in arrary
                data_ul(counter)=ul_count;
                data_rir(counter)= sensordata(counter,1);
                data_lir(counter)= sensordata(counter,2);
                
                if data_move(counter) == 'q'
                    confirm_unloading=1;
                    location = [2,1];
                    p(map_big==22)=0.04;
                    p(8,2)= 0.6;
                    p(8,3) = 0.5;
                    
                    p(map_big~=22)=0.005;
                    heading = 270;
                end
                
%% heading probability
%                 %not gonna change heading again when the once localized
%                 if confirm_local ==0
%                     headingguess = headchange (headingguess, data_move(counter));
%                     %sensor update
%  
%                     p1 = sense_2r_u(bw,ultra, M, p1, data_lir(counter),data_rir(counter),data_ul(counter),headingguess(1));
%                     p2 = sense_2r_u(bw,ultra, M, p2, data_lir(counter),data_rir(counter),data_ul(counter),headingguess(2));
%                     p3 = sense_2r_u(bw,ultra, M, p3, data_lir(counter),data_rir(counter),data_ul(counter),headingguess(3));
%                     p4 = sense_2r_u(bw,ultra, M, p4, data_lir(counter),data_rir(counter),data_ul(counter),headingguess(4));
%                     %calculate probability for heading find the max probability
%                     %in all four directions
%                     m = 0.2*ones(4,1);
%                     pmax(1) = max(p1(:)) ;
%                     pmax(2) = max(p2( :)) ;
%                     pmax(3) = max(p3(:)) ;
%                     pmax(4) = max(p4(:)) ;
%                     %which one is the most probability in four direction and
%                     %assign it 0.6
%                     [mh,I]=max(pmax);
%                     m(I) = 0.4;
%                     %apply to the probability and normalize it
%                     pheading = pheading.*m;
%                     pheading = pheading./sum(pheading);
%                     [pheading_max, pheading_index] = max(pheading);
% 
%                     disp(pheading);
%                     
%                     if data_move(counter) == 'w'
%                         move_counter = move_counter +0.5;
%                     elseif data_move(counter) == 'f' 
%                         move_counter = move_counter +1;
%                     end
% 
%                     if ( move_counter >= 2)
%               
%                         [p1] = move_only(p1, M, headingguess(1), data_move(counter));
%                         [p2] = move_only(p2, M, headingguess(2), data_move(counter));
%                         [p3] = move_only(p3, M, headingguess(3), data_move(counter));
%                         [p4] = move_only(p4, M, headingguess(4), data_move(counter));
%                         if move_counter == 2
%                             move_counter = 0;
%                         end
%                     end
%                     
%                     subplot (2,3,1);                        
%                     imagesc(p1);title(headingguess(1));
%              
%                     subplot (2,3,2);                    
%                     imagesc(p2);title(headingguess(2));
%  
%                     subplot (2,3,3);                    
%                     imagesc(p3);title(headingguess(3));
%             
%                     subplot (2,3,4);                    
%                     imagesc(p4);title(headingguess(4));
%         
%                     subplot (2,3,5);                    
%                     imagesc(pheading);title("pheading");
%                     pause(0.5);
%                                         
%                     disp("checking heading");
%                     
%                     if data_ul(counter) == 0                       
%                         if sensordata(counter,3)>30 && sensordata(counter,3)<40
%                             if sensordata(counter,6)>60 && sensordata(counter,8)>60 && sensordata(counter,9)>60
%                                 confirm_heading = 1;
%                                 heading = 90;
%                                 p(ultra ~= 0) = 0;
%                                 p(ultra == 0) = 0.06;  
%                                 confirm_local = 1;
%                                 disp("+ cross confirm");
%                             end
%                         elseif sensordata(counter,6)>30 && sensordata(counter,6)<40
%                             if sensordata(counter,3)>60 && sensordata(counter,8)>60 && sensordata(counter,9)>60
%                                 confirm_heading = 1;
%                                 heading = 180;
%                                 p(ultra ~= 0) = 0;
%                                 p(ultra == 0) = 0.06;   
%                                 confirm_local = 1;
%                                 disp("+ cross confirm");
%                             end
%                         elseif sensordata(counter,8)>30 && sensordata(counter,8)<40
%                             if sensordata(counter,3)>60 && sensordata(counter,6)>60 && sensordata(counter,9)>60
%                                 confirm_heading = 1;
%                                 heading = 0;
%                                 p(ultra ~= 0) = 0;
%                                 p(ultra == 0) = 0.06;        
%                                 confirm_local = 1
%                                 disp("+ cross confirm");
%                             end
%                         elseif sensordata(counter,9)>30 && sensordata(counter,9)<40
%                             if sensordata(counter,3)>60 && sensordata(counter,6)>60 && sensordata(counter,8)>60
%                                 confirm_heading = 1;
%                                 heading = 270;
%                                 p(ultra ~= 0) = 0;
%                                 p(ultra == 0) = 0.06;  
%                                 confirm_local = 1;   
%                                 disp("+ cross confirm");
%                             end                            
%                         end
%                     end
%  
%                     if (max(pheading)>0.9 && counter >10 && confirm_heading ==0)
%                         disp("confirm heading/change heading")
%                         confirm_heading = 1;
%                         heading = headingguess(pheading_index);
%                         confirm_heading_index = pheading_index;
%                         disp(heading);
%                         if pheading_index == 1
%                             p = p1;
%                         elseif pheading_index == 2
%                             p = p2;
%                         elseif pheading_index == 3
%                             p = p3;
%                         elseif pheading_index == 4
%                             p = p4;
%                         end
%                         
%                         if I ~= pheading_index
%                             heading_reset_counter = heading_reset_counter+1;
%                         elseif heading_reset_counter>0
%                             heading_reset_counter = heading_reset_counter-1;
%                         end
%                         
%                         if heading_reset_counter == 3
%                             pheading = ones(4,1);
%                             pheading(confirm_heading_index) = 0;
%                             confirm_heading = 0;
%                         end                                                   
%                     end
%                 end
%% calculate proability to localize
                %assume now known heading, process localization 
                if confirm_heading == 1
                    close all;                        
                    disp("known heading")
                    %if it is localized do not headchange base on the robot
                    %feedback
                    if confirm_local == 0
                        heading = headchange (heading, data_move(counter));
                    end
                    if data_move(counter) == 'w' || data_move(counter) == 'f' || data_move(counter) == 'r' || data_move(counter) == 'l'
                        p=sense_2r_u(bw,ultra, M, p,  data_lir(counter),data_rir(counter),data_ul(counter),heading);
                    
                        subplot (2,1,1);                    
                        imagesc(p);title(heading);
                        pause(0.5);
                        %find the max point of the probability 
                        max_p = max(p(:));
                        %extra the location index
                        [current_row, current_col] = find(ismember(p,max(p(:))));
                        %get the location stored in the new location
                        prelocation = location;
                        %get the large block new location
                        location=[ceil(current_row/4),ceil(current_col/4)];
                        % the location diff
                        if length(prelocation) == length(location)
                            loc_diff = prelocation-location;
                        end
                        
                    end

                    

                    %movement counter
                    if data_move(counter) == 'w'
                        move_counter = move_counter +0.5;
                    elseif data_move(counter) == 'f' 
                        move_counter = move_counter +1;
                    end
                    %movement
                    if ( move_counter >= 2)
                        p=move_only(p, M, heading, data_move(counter));                       
                        move_counter = move_counter-2;

                    end

%                     disp(data_move(counter));
%                     disp(data_ul(counter));
%                     disp(data_lir(counter));
%                     disp(data_rir(counter));
                    %disp(location);

                    
                    if data_ul(counter) == 0 && confirm_local == 0
                        disp("at +cross");
                        %cross_counter=0;
                        if sensordata(counter,3)>30 && sensordata(counter,3)<45
                            disp("90");
                            if sensordata(counter,6)>60 && sensordata(counter,8)>60 && sensordata(counter,9)>60
                                confirm_heading = 1;
                                %heading = 90;
                                p(ultra~=0)=0.005;
                                p(ultra == 0) = 0.06;  
                                fprintf(bt,'9');
                                location = [2, 6];
                                disp("+ cross confirm");
                            end
                        elseif sensordata(counter,6)>30 && sensordata(counter,6)<45
                            disp("180");
                            if sensordata(counter,3)>60 && sensordata(counter,8)>60 && sensordata(counter,9)>60
                                confirm_heading = 1;
                                %heading = 180;
                                p(ultra~=0)=0.005;
                                p(ultra == 0) = 0.06;   
                                fprintf(bt,'9');
                                location = [2, 6];
                                disp("+ cross confirm");
                            end
                        elseif sensordata(counter,8)>30 && sensordata(counter,8)<45
                            disp("0");
                            if sensordata(counter,3)>60 && sensordata(counter,6)>60 && sensordata(counter,9)>60
                                confirm_heading = 1;
                                %heading = 0;
                                p(ultra~=0)=0.005;
                                p(ultra == 0) = 0.06;        
                                fprintf(bt,'9');
                                location = [2, 6];
                                
                                disp("+ cross confirm");
                            end
                        elseif sensordata(counter,9)>30 && sensordata(counter,9)<45
                            disp("270");
                            if sensordata(counter,3)>60 && sensordata(counter,6)>60 && sensordata(counter,8)>60
                                confirm_heading = 1;
                                %heading = 270;
                                p(ultra~=0)=0.005;
                                p(ultra == 0) = 0.06;  
                                fprintf(bt,'9');
                                location = [2, 6];
                                disp("+ cross confirm");
                            end                            
                        end
                    end

                    
                    

                    %determine the localization factor, and add counter                    
                    if max_p >0.05 && max(abs(loc_diff(:)))<1 && counter > 15  && max(pheading)>0.9 && confirm_local ==0
                        disp("local!")                        
                        local_counter = local_counter+1;
                        disp(local_counter);
%                     elseif max(abs(loc_diff(:)))>2 && counter >10
%                         confirm_heading = 0;
                    end
                    %confirm localization when counter reach 8 times
                    if local_counter == 5                        
                        fprintf(bt,'9');
                        disp("localization complete, light on!")
%                         confirm_local = 1;
                    end
                    
                    if data_move(counter) == 'g'
                        confirm_local = 1;
                    end

%                     if confirm_local == 2
%                         confirm_local = 1;                        
%                         
%                     end
                % elseif confirm_heading == 2
                end        
                %% after localized       
                
                if confirm_local == 1 
                    if confirm_loading == 0 || confirm_unloading == 1 
                        disp("pathfinding");
                        disp(heading);
                        path_start = map(location(1,1),location(1,2));

                        if  confirm_loading == 0

                            [distance_1, path_1] = dijkstra(nodes, segments,path_start ,15);
                            [distance_2, path_2] = dijkstra(nodes, segments,path_start ,23);

                            if distance_1 < distance_2
                                path = path_1;
                            else
                                path = path_2;
                            end
                        elseif confirm_unloading == 1
                            [distance, path] = dijkstra(nodes, segments,path_start ,path_final);
                        end

                        subplot (2,1,2);
                        plot(nodes(:,2), nodes(:,3),'k.');
                        xlim([0 9]) ;
                        ylim([0 5]);
                        hold on;
                        for n = 2:length(path)
                            plot(nodes(path(n-1:n),2),nodes(path(n-1:n),3),'r-.','linewidth',2);
                        end
                        pause(0.5);
                        hold off;

                        if length(path) == 1
                            if confirm_loading == 0
                                pause(2);
                                fprintf(bt,'8');
                                disp("at loading zone !!, light on !!")                 
                                confirm_loading = 1;
                                pause(2);
                                fprintf(bt,'z');                        
                                disp ("begin loading !!")
                            end
                            if confirm_unloading == 1
                                pause(2);
                                fprintf(bt,'7');
                                disp("at unloading zone !!, light on !!")   
                                confirm_unloading =2;
                            end                               
                            
                        else 
                            [target_row,target_col]=find(map==path(2));
                            diff_row = target_row - location(1,1);
                            diff_col = target_col - location(1,2);

                            %1-forward, 2-right, 3-left, 6-uturn
                            disp([target_row target_col]);
                            disp(location);
                            disp("give command");                   
                            if heading == 0
                                if diff_row > 0
                                    heading = headchange (heading, 'r');
                                     command = '2'; 
                                     fprintf(bt,command); 
                               elseif diff_row <0
                                    heading = headchange (heading, 'l');                           
                                     command = '3';
                                     fprintf(bt,command);   
                                elseif diff_col >0
                                     command = '1';
                                     fprintf(bt,command);   
                                elseif diff_col <0
                                    heading = headchange (heading, 'b');
                                     command = '6'; 
                                     fprintf(bt,command); 
                                end
                            elseif heading == 90
                                if diff_row > 0
                                    heading = headchange (heading, 'b');
                                     command = '6';
                                     fprintf(bt,command);  
                                elseif diff_row <0
                                     command = '1';
                                     fprintf(bt,command);  
                                elseif diff_col >0 
                                    heading = headchange (heading, 'r');
                                     command = '2';
                                     fprintf(bt,command);  
                                elseif diff_col <0
                                    heading = headchange (heading, 'l');
                                     command = '3';
                                     fprintf(bt,command);   
                                end                               
                            elseif heading == 180
                                if diff_row > 0
                                    heading = headchange (heading, 'r');
                                     command = '2';
                                     fprintf(bt,command); 
                                elseif diff_row <0
                                    heading = headchange (heading, 'l');
                                     command = '3';
                                     fprintf(bt,command);  
                                elseif diff_col >0
                                    heading = headchange (heading, 'b');
                                     command = '6';
                                     fprintf(bt,command);   
                                elseif diff_col <0
                                     command = '1';
                                     fprintf(bt,command);   
                                end
                            else
                                if diff_row > 0
                                     command = '1';
                                     fprintf(bt,command);   
                                elseif diff_row <0
                                    heading = headchange (heading, 'b');
                                     command = '6';
                                     fprintf(bt,command);   
                                elseif diff_col >0
                                    heading = headchange (heading, 'l');
                                     command = '3';
                                     fprintf(bt,command);  
                                elseif diff_col <0
                                    heading = headchange (heading, 'r');
                                     command = '2';       
                                     fprintf(bt,command);   
                                end
                            end  
                            pause(0.5);
                            disp(command);
                            disp(heading);
                            disp(diff_row);
                            disp(diff_col);



                %                 a=~ismember(map_big,path);
                %                 
                %                 for i=1:dim2
                %                     for j=1:dim1
                %                         if a(i,j)== 1
                %                             p(i,j) = 0.001;
                %                         end
                %                     end
                %                 end
                %                 

                        end
            %             confirm_local =2;
                    end
                end
                
                counter =counter+1;
                
                
            end

        end
 
      
    end
    sample = " ";
end

