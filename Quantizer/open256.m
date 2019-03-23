% open 256
clc; clear; close all;

%% chem
% chem = importdata('chem.dat');
% chem = reshape(chem, [256, 256]);
% figure;
% histogram(chem, 0 : 1 : 256);
% axis([0 256 0 inf]);
% title('Histogram of chem.256');
% xlabel('Intensity'); ylabel('Number of Pixels');

%% house
% house = importdata('house.dat');
% house = reshape(house, [256, 256]);
% figure;
% histogram(house, 0 : 1 : 256);
% axis([0 256 0 inf]);
% title('Histogram of house.256');
% xlabel('Intensity'); ylabel('Number of Pixels');

%% moon
% moon = importdata('moon.dat');
% moon = reshape(moon, [256, 256]);
% figure;
% histogram(moon, 0 : 1 : 256);
% axis([0 256 0 inf]);
% title('Histogram of moon.256');
% xlabel('Intensity'); ylabel('Number of Pixels');

%% quantized
chem_2bit = importdata('chem_2bit.dat');
chem_4bit = importdata('chem_4bit.dat');
couple_2bit = importdata('couple_2bit.dat');
couple_4bit = importdata('couple_4bit.dat');
elaine_2bit = importdata('elaine_2bit.dat');
elaine_4bit = importdata('elaine_4bit.dat');
f16_2bit = importdata('f16_2bit.dat');
f16_4bit = importdata('f16_4bit.dat');
house_2bit = importdata('house_2bit.dat');
house_4bit = importdata('house_4bit.dat');
moon_2bit = importdata('moon_2bit.dat');
moon_4bit = importdata('moon_4bit.dat');

quantizedImages = [chem_2bit, couple_2bit, elaine_2bit, f16_2bit,...
    house_2bit, moon_2bit, chem_4bit, couple_4bit, elaine_4bit,...
    f16_4bit, house_4bit, moon_4bit];
quantizedImages = mat2cell(quantizedImages, size(quantizedImages, 1),...
    ones(1, size(quantizedImages, 2)));
titles = [{'2-bit Quantized Histogram of chem.256'},...
    {'2-bit Quantized Histogram of couple.256'},...
    {'2-bit Quantized Histogram of elaine.256'},...
    {'2-bit Quantized Histogram of f16.256'},...
    {'2-bit Quantized Histogram of house.256'},...
    {'2-bit Quantized Histogram of moon.256'},...
    {'4-bit Quantized Histogram of chem.256'},...
    {'4-bit Quantized Histogram of couple.256'},...
    {'4-bit Quantized Histogram of elaine.256'},...
    {'4-bit Quantized Histogram of f16.256'},...
    {'4-bit Quantized Histogram of house.256'},...
    {'4-bit Quantized Histogram of moon.256'}];

for i = 1 : length(quantizedImages)
    quantizedImages{i} = reshape(quantizedImages{i}, [256, 256]);
    figure;
    histogram(quantizedImages{i}, 0 : 1 : 256);
    axis([0 256 0 inf]);
    xlabel('Intensity'); ylabel('Number of Pixels');
    title(titles{i});
end

