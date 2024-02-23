echo g++ -std=c++17 SSOCs.cpp -I ~/git/EiCOS/include/ -L ~/git/EiCOS/ -l eicos_LDBL -Wl,-rpath,$HOME/git/EiCOS/ -Ofast -o SSOCs_LDBL.exe
g++ -std=c++17 SSOCs.cpp -I ~/git/EiCOS/include/ -L ~/git/EiCOS/ -l eicos_LDBL -Wl,-rpath,$HOME/git/EiCOS/ -Ofast -o SSOCs_LDBL.exe
