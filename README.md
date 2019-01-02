# Viewer for boat cameras
![alt text](https://raw.githubusercontent.com/julled/boatCamViewer/master/screenshot.jpg)
* Features
  - Horizont detection: Automatically detect horizont 
    - Implemented naive gradient threshold algorithm
  - Horizont correction: Rotate and translate image in a way that horizont is at center of the image
    - Possible to speed up by using CUDA functions: add "USE_CUDA" define in pro file
  - Object tracking: Track objects by drawing box around them 
    - using OpenCV trackers
* Dependencys
  - QT
  - OpenCV 3

Videos:
https://cloud.hs-augsburg.de/index.php/s/ddCsSjJZoRRi5Tq
