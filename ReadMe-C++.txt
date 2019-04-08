You will find

- A program to load and display the first frame of a .rgb video file. This has been
given as Microsoft Visual C++ .dsp and .dsw project files along with
the accompanying code in the .cpp and .h files

- The program creates a single image containing the first frame of the video.

The project includes the following important files.
1. Image.h	- header file for MyImage class
2. Image.cpp	- defines operations on an image such as read, write, modify
3. Main.cpp	- entry point of the application, takes care of the GUI, the 
		  image display operations, and the initialization of the RGB image.

Some indicators have been placed in the code to guide you to develop your code. But you
you are free to modify the program in any way to get the desirable output.

- Unzip the folder in your desired location
- Launch Visual Studio and load the .dsw or .dsp project files
- If you use the .net compiler, you can still load the project file, just 
  follow the default prompts and it will create a solution .sln file for you
- Compile the program to produce a an executable Image.exe
- To run the program you need to provide the program with command line arguments, they can 
  be passed in Visual C++ using Project > Settings or just launch a .exe file from the command prompt
- Here is the usage:

Image.exe \path\to\video.rgb