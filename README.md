# tartan
LGE Security Specialist Course - Studio Project

#Ports  
Non-Secure TCP Port: 50000  
Secure TCP port: 55555

#User  
admin - Learning, running, testrun mode possible  
lg - running, testrun mode possible

# 학습모드  
사람당 학습가능한 이미지샘플수: 5-8 (정면, 좌우,위아래)  
클라이언트는 사람이름, 샘플수 넣고 시작 누르면 서버가 다되었다는 메시지 알려주기 전까지는 서버로부터 몇장 저장되었다는 메시지만 받고 화면에 띄움  
서버가 학습이 끝났다고 알려주면 그 때 학습 모드 종료  



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

cd ~/work/tartan/LgFaceRecDemoTCP_Jetson_NanoV2

sudo tartan_install.sh



