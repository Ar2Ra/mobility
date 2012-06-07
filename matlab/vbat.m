%% Create the serial object
serialPort = 'COM25';
serialObject = serial(serialPort, 'BaudRate', 115200);
fopen(serialObject);

%% Set up the figure window
time = 0;
voltage = 0;
sample = 0;

figureHandle = figure('NumberTitle','off',...
    'Name','Tensiunea bateriei',...
    'Visible','off');

% Set axes
axesHandle = axes('Parent',figureHandle,'YGrid','on','XGrid','on');

hold on;

plotHandle = plot(axesHandle,time,voltage,'LineWidth',2);

%xlim(axesHandle,[min(time) max(time+0.001)]);

% Create xlabel
xlabel('Timp [s]','FontWeight','bold','FontSize',14,'Color',[0 0 1]);

% Create ylabel
ylabel('Tensiune [mV]','FontWeight','bold','FontSize',14,'Color',[0 0 1]);

% Create title
title('Masurarea tensiunii la baterie','FontSize',15,'Color',[0 0 1]);

% microcontroller - time between each value sent on RS232
sampleTime = 0.5;

% interval pause for PC data collection
pauseInterval = sampleTime / 2;

%% Collect data
count = 1;

set(figureHandle,'Visible','on');

hf=figure('position',[0 0 eps eps],'menubar','none');

while 1
    ch = get(hf,'currentcharacter');
    
    if strcmp(ch, 'q') %quit
        close(hf)
        break
    end
    
    if strcmp(ch, 'c') %clear
        time = 0;
        voltage = 0;
        count = 1;
    end
    
    sample = fscanf(serialObject, '%d\n');

    time(count) = count * sampleTime;   
    voltage(count) = sample;
    
    set(plotHandle, 'YData', voltage, 'XData', time);

    pause(pauseInterval);
    count = count + 1;

end

%% Clean up the serial object
fclose(serialObject);
delete(serialObject);
clear serialObject;
%clear all;
