
----------------------------------------------
Installing libpng-1.6.35 on your system with CMAKE
----------------------------------------------
    cmake -DBUILD_SHARED_LIBS=ON .
    make
    make install
    ldconfig


----------------------------------------------
Installing jsoncpp-1.7.5 on your system with CMAKE
----------------------------------------------

    cmake -DBUILD_SHARED_LIBS=ON .
    make
    make install
       

----------------------------------------------
Installing GLFW 3.1 on your system with CMAKE
----------------------------------------------

    --------------------------------
    Step 1: Installing GLFW 3 on your system with CMAKE
    --------------------------------

        For this install, I was using Ubuntu 16.04, 64bit.

        The first step is to download the latest version (assuming versions in the future work in a similar way) from www.glfw.org, probably using this link.

        The next step is to extract the archive, and open a terminal. cd into the glfw-3.X.X directory and run  cmake -DBUILD_SHARED_LIBS=ON . you may need elevated privileges, and you may also need to install build dependencies first. To do this, try sudo apt-get build-dep glfw or sudo apt-get build-dep glfw3 or do it manually, as I did using sudo apt-get install cmake xorg-dev libglu1-mesa-dev... There may be other libs you require such as the pthread libraries... Apparently I had them already. (See the -l options given to the g++ linker stage, below.)

        Now you can type make and then make install, which will probably require you to sudo first.

        Okay, you should get some verbose output on the last three CMake stages, telling you what has been built or where it has been placed. (In /usr/include, for example.)


    --------------------------------
    Step 2: Create a test program and compile
    --------------------------------
        The next step is to fire up vim ("what?! vim?!" you say) or your preferred IDE / text editor... I didn't use vim, I used Kate, because I am on KUbuntu 13.04... Anyway, download or copy the test program from here (at the bottom of the page) and save, exit.

        Now compile using g++ -std=c++11 -c main.cpp - not sure if c++11 is required but I used nullptr so, I needed it... You may need to upgrade your gcc to version 4.7, or the upcoming version 4.8... Info on that here.

        Then fix your errors if you typed the program by hand or tried to be "too clever" and something didn't work... Then link it using this monster! g++ main.o -o main.exec -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi So you see, in the "install build dependencies" part, you may also want to check you have the GL, GLU, X11 Xxf86vm (whatever that is) Xrandr posix-thread and Xi (whatever that is) development libraries installed also. Maybe update your graphics drivers too, I think GLFW 3 may require OpenGL version 3 or higher? Perhaps someone can confirm that? You may also need to add the linker options -ldl -lXinerama -lXcursor to get it to work correctly if you are getting undefined references to dlclose (credit to @user2255242).

        And, yes, I really did need that many -ls!

    --------------------------------
    Step 3: You are finished, have a nice day!
    --------------------------------
