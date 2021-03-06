# ChatServer
-> a server in C++ that echos the client's messages back. It allows multiple [clients](https://github.com/Wtf-Is-This-x1337/ChatClient) to connect using the fd_set structure, the select() function and three macros: FD_CLR, FD_SET and FD_ZERO
## Images:    
-> Settings         
![alt text](https://github.com/Wtf-Is-This-x1337/ChatServer/blob/main/images/setup.png?raw=true)
              
-> Main && Connection example (username, message, command)         
![alt text](https://github.com/Wtf-Is-This-x1337/ChatServer/blob/main/images/main.png?raw=true)     

## How to build
1. Download the file (code -> download ZIP)
2. Extract it
3. Go to ChatClient/
4. Run ChatClient.sln (I use Visual Studio 2019)
5. Build solution on Debug / x86

## How to use
1. Run .\Debug\Server.exe
2. use an unused local port (ex: 54000)
3. wait for connections (example: from [ChatClient](https://github.com/Wtf-Is-This-x1337/ChatClient))

## License
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this software, either in source code form or as a compiled binary, for any purpose, commercial or non-commercial, and by any means. [The unlicense]
