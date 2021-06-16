mkdir -p /usr/local/tartan/bin
cp tartan.sh /usr/local/tartan/bin/
cp build/LgFaceRecDemoTCP_Jetson_NanoV2 /usr/local/tartan/bin/
cp *.mp4 /usr/local/tartan/
cp tartan*.db /usr/local/tartan/
cp mtCNNModels /usr/local/tartan/ -rf
cp facenetModels /usr/local/tartan/ -rf
cp facenet.pb /usr/local/tartan/
cp shinpark /var/ -rf
cp tartan.service /lib/systemd/system/

