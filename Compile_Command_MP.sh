echo g++ -std=c++17 SSOCs.cpp -I ~/git/EiCOS/include/ -L ~/git/EiCOS/ -l eicos_MP -Wl,-rpath,$HOME/git/EiCOS/ -Ofast -o SSOCs.exe
g++ -std=c++17 SSOCs.cpp -I ~/git/EiCOS/include/ -L ~/git/EiCOS/ -l eicos_MP -Wl,-rpath,$HOME/git/EiCOS/ -Ofast -o SSOCs.exe
