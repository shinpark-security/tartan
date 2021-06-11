g++ RecvImageTCP.cpp ../Common/NetworkTCP.cpp ../Common/TcpSendRecvJpeg.cpp ../Common/Protocol/protocol.pb.cc ../Common/Protocol/ProtoBuff.cpp -o RecvImageTCP -I../Common -L/usr/local/lib -I/usr/local/include/opencv4/ -lprotobuf -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio -lwolfssl 


