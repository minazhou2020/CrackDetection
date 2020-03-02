# CrackDetection
## About this project
This repository contains the code for crack detection in asphalt surfaces. It is a c++, opencv implementation of the paper by Yuxiao Zhou:
[Seed-Based Approach for Automated Crack Detection from Pavement Images](https://www.researchgate.net/publication/305792615_Seed-Based_Approach_for_Automated_Crack_Detection_from_Pavement_Images)"".

- The proposed crack detection algorithm includes following steps:
![crack detection methodology](https://github.com/minazhou2020/CrackDetection/blob/master/Capture.PNG?raw=true)

- Image Background Correction
Pavement images are taken under different weather conditions and daytime, and may contain shadows and large variations of texture. As a result non-uniform lighting is present in the images. It is necessary to standardize image background in order to enhance the accuracy of crack detection.

![image background correction](https://github.com/minazhou2020/CrackDetection/blob/master/description/backgroundCorrection.PNG?raw=true)

- Crack Seed Verification
We employed the Grid Cell Analysis (GCA) method described in for seed detection, as this algorithm is capable of improving the detection speed by reducing the size of search space. A preprocessed image was divided into grid cells of 8×8 pixels, and the cells were classified as either a non-crack cell or a crack seed by analyzing brightness changes on neighboring cells. A group of patterns were  proposed adopted for crack seed verification. 

![crack seed verification patterns](https://github.com/minazhou2020/CrackDetection/blob/master/description/Patterns.PNG?raw=true)

- Seed Clustering
In seed clustering, individual seeds were connected into seed clusters using a Minimum Spanning Tree (MST) based method. A MST is a minimum weight, cycle free subset of a graph's edges such that all nodes are connected. 

![seed clustering](https://github.com/minazhou2020/CrackDetection/blob/master/description/SeedDetection.PNG?raw=true)

## Prerequisites

OpenCV

## Dataset 
- The data set can be downloaded from this [samples](https://github.com/minazhou2020/CrackDetection/tree/master/linear_cracks)

## Installation

## Usage

|Argument Index | Description                   |
| ------------- |:-----------------------------:|
| 1             |  Folder cotaining crack images|
## TO DO
- Lane Marking Detection
- Undesirable particles removal
## Author
Yuxiao Zhou


