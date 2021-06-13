g++ RecvImageTCP.cpp  ../Common/NetworkTCP.cpp  ../Common/TcpSendRecvJpeg.cpp  ../Common/Protocol/MyProtocol.cpp ../Common/Protocol/ProtocolManager.cpp ../Common/Protocol/BaseProtocol.cpp ../Common/Protocol/Msg/protocolLogin.pb.cc  -o RecvImageTCP -I../Common -I../Common/Protocol -I../Common/Protocol/Msg -L/usr/local/lib -I/usr/include/opencv4/ -I/usr/local/include/opencv4/ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include  -lprotobuf -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio -lwolfssl  -lglib-2.0


