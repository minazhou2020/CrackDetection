# CrackDetection
## About this project
This repository contains the code for crack detection in asphalt surfaces. It is a c++, opencv implementation of the paper by Yuxiao (Mina) Zhou:
[Seed-Based Approach for Automated Crack Detection from Pavement Images](https://www.researchgate.net/publication/305792615_Seed-Based_Approach_for_Automated_Crack_Detection_from_Pavement_Images)"".

- The proposed crack detection algorithm includes following steps:
![crack detection methodology](https://github.com/minazhou2020/CrackDetection/blob/master/Description/Capture.PNG?raw=true)

- Image Background Correction

  Pavement images are taken under different weather conditions and daytime, and may contain shadows and large variations of texture. As a result non-uniform lighting is present in the images. It is necessary to standardize image background in order to enhance the accuracy of crack detection.

![image background correction](https://github.com/minazhou2020/CrackDetection/blob/master/Description/backgroundCorrection.PNG?raw=true)

- Crack Seed Verification

  We employed the Grid Cell Analysis (GCA) method described in for seed detection, as this algorithm is capable of improving the detection speed by reducing the size of search space. A preprocessed image was divided into grid cells of 8×8 pixels, and the cells were classified as either a non-crack cell or a crack seed by analyzing brightness changes on neighboring cells. A group of patterns were  proposed adopted for crack seed verification. 

![crack seed verification patterns](https://github.com/minazhou2020/CrackDetection/blob/master/Description/Patterns.PNG?raw=true)

- Seed Clustering

  In seed clustering, individual seeds were connected into seed clusters using a Minimum Spanning Tree (MST) based method. A MST is a minimum weight, cycle free subset of a graph's edges such that all nodes are connected. 

![seed clustering](https://github.com/minazhou2020/CrackDetection/blob/master/Description/SeedDetection.PNG?raw=true)

## Prerequisites

OpenCV

## Installation
- Install OpenCV 4
Install OpenCV 4 following instructions on [opencv installation guide](https://docs.opencv.org/master/d3/d52/tutorial_windows_install.html#tutorial_windows_install_prebuilt)

## Usage

| Argument Index |          Description           |
| -------------- | :----------------------------: |
| 1              | Folder containing crack images |


To run the program in command prompt

Open command prompt -> Got to the download `CrackDetection.exe`( download [here](https://github.com/minazhou2020/CrackDetection/blob/master/Exe/CrackDetection.exe))'s location using `cd` command 

 execute the downloaded `CrackDetection.exe`  with the following command:

`CrackDetection path\to\image_directory`

## TO DO

- Provide test image set 
- Lane marking detection
- Undesirable particles removal
## Author
Yuxiao (Mina) Zhou


