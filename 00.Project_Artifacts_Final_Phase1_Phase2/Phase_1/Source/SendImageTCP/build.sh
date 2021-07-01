g++ SendImageTCP.cpp ../Common/NetworkTCP.cpp ../Common/Protocol/ProtoBuff.cpp ../Common/Protocol/protocol.pb.cc ../Common/TcpSendRecvJpeg.cpp -o SendImageTCP -I/usr/local/include/opencv4/ -I../Common -L/usr/local/lib -lprotobuf -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio


