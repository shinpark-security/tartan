g++ -g Login.cpp ../Common/Protocol/LoginProtocol.cpp ../Common/Protocol/Msg/protocolLogin.pb.cc ../LgFaceRecDemoTCP_Jetson_NanoV2/src/NetworkTCP.cpp -o Login -I/usr/include -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/include/opencv4/ -I../Common -L/usr/local/lib -lglib-2.0 -lwolfssl -lprotobuf -lprotobuf-lite -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio


