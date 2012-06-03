%% Real time data collection example
% This MATLAB script generates a real time plot of voltage values collected
% from an Agilent(TM) 34401A Digital Multimeter over the serial
% port(RS232). The data is collected and plotted until the predefined stop
% time is reached. This example also demonstrates automating a query based
% interaction with an instrument while monitoring the results live.
%
% This script can be modified to be used on any platform by changing the
% serialPort variable.
% 
% Example:-
%
% On Linux:     serialPort = '/dev/ttyS0';
%
% On MacOS:     serialPort = '/dev/tty.KeySerial1';
%
% On Windows:   serialPort = 'COM1';
%
%
% The script may also be updated to use any instrument/device to collect
% real time data. You may need to update the SCPI commands based on
% information in the instrument's programming manual.
%
% To generate a report of this entire script, you may use the PUBLISH
% command at the MATLAB(R) command line as follows: 
% 
% publish(real_time_data_plot);
% 
% Author: Ankit Desai
% Copyright 2009 - The MathWorks, Inc.

%% Create the serial object
serialPort = 'COM28';
serialObject = serial(serialPort);
fopen(serialObject);

%% Set up the figure window
time = 0;
voltage = 0;
sample = 0;

figureHandle = figure('NumberTitle','off',...
    'Name','Voltage Characteristics',...
    'Color',[0 0 0],'Visible','off');

% Set axes
axesHandle = axes('Parent',figureHandle,...
    'YGrid','on',...
    'YColor',[0.9725 0.9725 0.9725],...
    'XGrid','on',...
    'XColor',[0.9725 0.9725 0.9725],...
    'Color',[0 0 0]);

hold on;

plotHandle = plot(axesHandle,time,voltage,'Marker','.','LineWidth',1,'Color',[0 1 0]);

%xlim(axesHandle,[min(time) max(time+0.001)]);

% Create xlabel
xlabel('Time','FontWeight','bold','FontSize',14,'Color',[0 0 1]);

% Create ylabel
ylabel('Motor angular speed','FontWeight','bold','FontSize',14,'Color',[0 0 1]);

% Create title
title('Motor Data Acquisition','FontSize',15,'Color',[0 0 1]);

% microcontroller - time between each values sent
sampleTime = 0.1;

% interval for PC data collection
pauseInterval = sampleTime / 2;

%% Collect data
count = 1;

set(figureHandle,'Visible','on');

hf=figure('position',[0 0 eps eps],'menubar','none');

while 1
    if strcmp(get(hf,'currentcharacter'),'q')
        close(hf)
        break
    end
    
    hline(90, 'r'); %set command plant

    sample = fscanf(serialObject, '%d');
    if sample < 0
        vline(count * sampleTime, 'b');
    else
        time(count) = count * sampleTime;
        voltage(count) = sample;

        set(plotHandle, 'YData', voltage, 'XData', time);

        pause(pauseInterval);
        count = count + 1;
    end
end

%% Clean up the serial object
fclose(serialObject);
delete(serialObject);
clear serialObject;
clear all;
