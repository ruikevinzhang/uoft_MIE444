nodes = [1 8 4; 2 8 3; 3 8 2; 4 8 1; 5 7 3; 6 6 4; 7 6 3; 8 6 2; 9 6 1; 10 5 3; 11 5 1; 12 4 4; 13 4 3; 14 4 1; 15 3 4; 16 3 2; 17 3 1; 18 2 4; 19 2 3; 20 2 1; 21 1 4; 22 1 3; 23 1 2; 24 1 1];
segments = [1 1 2; 1 2 3; 1 2 5; 1 3 4; 1 5 7; 1 7 6; 1 7 8; 1 7 10; 1 8 9; 1 9 11; 1 10 13; 1 11 14; 1 12 13; 1 12 15; 1 14 17; 1 15 18; 1 16 17; 1 17 20; 1 18 19; 1 18 21; 1 19 22; 1 20 24; 1 21 22; 1 22 23; 1 23 24];
map = [21 18 15 12 0 6 0 1; 22 19 0 13 10 7 5 2; 23 0 16 0 0 8 0 3; 24 20 17 14 11 9 0 4];

figure; plot(nodes(:,2), nodes(:,3),'k.');
xlim([0 9]) 
ylim([0 5])

hold on;
for s = 1:25
    if (s <= 24) text(nodes(s,2),nodes(s,3),[' ' num2str(s)]); end
    plot(nodes(segments(s,2:3)',2),nodes(segments(s,2:3)',3),'k');
end
[d1, p1] = dijkstra(nodes, segments, 22,1)
[d2, p2] = dijkstra(nodes, segments, 22,1)

if d1 < d2
    path = p1
else
    path = p2
end

for n = 2:length(path)
    plot(nodes(path(n-1:n),2),nodes(path(n-1:n),3),'r-.','linewidth',2);
end