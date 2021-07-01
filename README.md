# tartan
LGE Security Specialist Course - Studio Project

#Ports  
Non-Secure TCP Port: 50000  
Secure TCP port: 55555

#User  
admin - Learning, running, testrun mode possible  
lg - running, testrun mode possible


# Artifacts for Phase 1 and Phase 2
https://github.com/shinpark-security/tartan/tree/main/Project_Artifacts_Final_Phase1_Phase2


# Build Environment
$ sudo apt-get install sqlite3 libsqlite3-dev libssl-dev libsystemd-dev


# wolfssl Installation
$ mkdir ~/work

$ cd ~/work

$ git clone https://github.com/wolfSSL/wolfssl.git

$ cd wolfssl

$ ./autogen.sh

$ ./configure --enable-keygen --enable-rsa --enable-ecc --enable-ed25519 --enable-des3 CFLAGS="-DWOLFSSL_PUBLIC_MP" --enable-all


# PROTOBUF Installation
Source download : https://developers.google.com/protocol-buffers/docs/downloads

$ tar xvf protobuf-all-3.17.3.tar.gz

$ cd protobuf-all-3.17.3

$ ./configure

$ make 

$ sudo make install

$ sudo ldconfig



# Server Installation and Run

$ cd ~/work/tartan/Project_Artifacts_Final_Phase1_Phase2/Phase_1/Source/LgFaceRecDemoTCP_Jetson_NanoV2/  

$ sudo tartan_install.sh

$ cd ~/work/tartan/firewall

$ sudo ./install.sh

$ sudo sync

$ sudo reboot



# CLIENT APP

github :

https://github.com/azzzzzzzzzzzzzzzzzzzzzzzzzzzzz/LGSecurity/



# Contact

Please register as an issue on this github where you find bugs or vulnerabilities in this program.  

If you have any reports or additional files, please send them to the email address below.  

dolnyumi@gmail.com
syblgeuser@gmail.com

