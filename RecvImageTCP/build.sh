g++ RecvImageTCP.cpp ../Common/NetworkManager.cpp ../Common/Protocol/ProtocolManager.cpp ../Common/Protocol/ImageProtocol.cpp ../Common/Protocol/LoginProtocol.cpp ../Common/NetworkTCP.cpp ../Common/TcpSendRecvJpeg.cpp ../Common/Protocol/Msg/protocolImage.pb.cc ../Common/Protocol/Msg/protocolLogin.pb.cc  -o RecvImageTCP -I../Common -I../Common/Protocol -I../Common/Protocol/Msg -L/usr/local/lib -I/usr/local/include/opencv4/ -lprotobuf -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio -lwolfssl 


