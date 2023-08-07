Mp3 Tagger
===================================================
  
This repo contains a basic program to tag one or more .mp3 files by using their title and a folder containing the album arts to set. To accomplish this I used Boost to parse the command line arguments and TagLib to tag the file.   

The .mp3 file should be named "authorName - songName", everything before the '-' will be treated as the author and everything after as the song title.

T the current time only .mp3 files are supported and only .jpg are supported as album cover arts.

I am by no means and expert in C++ so if you find any problems and/or want to propose a solution/different ways to do stuff feel free to get in touch with me and/or open an issue here on GitHub.

How to compile
----
***Linux:***

A CodeBlocks .cbp file is available, change the paths to the libraries accordingly with your local paths if needed and it should compile out of the box.

***Windows:***

A Visual Studio 2019 solution is available, change the paths to the libraries accordingly with your local paths if needed and it should compile out of the box.

It should also work with Visual Studio 2015 and 2017 but I haven't tested it
 
 How to run
----
To run the program move to the executable's directory and type ./mp3_tagger on Linux or mp3_tagger.exe on Windows, this program can take 7 arguments.  
Arguments in square brackets are optional.


- **[-h]**
- **[-t]**
- **[-c]**  
- **-f fileName**
- **-d dirName**
- **[-p picDirName]**
- **[-v]**
  
-h (--help) produces the help message with the list of arguments and their functionalities.  

-t (--tag) tags the selected .mp3 file/all the .mp3 files in the specified folder.  

-c (--clear) clears the tags from the selected .mp3 file/all the .mp3 files in the specified directory, if both -c and -t are selected the tags will be cleared before being set.

-f (--file) allows to select a single .mp3 file to tag

-d (--dir) allows to select all the .mp3 files in a directory to tag

-p (--picture) allows to select a directory containing the .jpg album arts

-v (--verbose) sets verbose output when tagging 

Album art setter
----
The .jpg files in the pictures folder should have the same name as the corresponding .mp3 file or the name of the artist (the string before '-' in the .mp3 file name), this is for convenience in case all the songs of a single artist should have the same album art.  

With this it's possible to set the same album cover art for more .mp3 files by only having a single .jpg file named like the author of the song.


Known bugs
----
If the song or the artist have '-' in their name the tagging will be wrong since I use the first occurrence of '-' to separate the artist and the song names.
