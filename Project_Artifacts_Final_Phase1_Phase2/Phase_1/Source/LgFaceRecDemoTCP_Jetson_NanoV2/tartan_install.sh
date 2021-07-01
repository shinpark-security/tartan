
mkdir -p /usr/local/tartan/bin
systemctl stop tartan
install build/LgFaceRecDemoTCP_Jetson_NanoV2 /usr/local/tartan/bin/ -v
install log.sh   /usr/local/tartan/bin/ -v
install restart_server.sh  /usr/local/tartan/bin/ -v
install *.mp4 /usr/local/tartan/ -v
install tartan*.db /usr/local/tartan/
install /usr/local/tartan/mtCNNModels -d -v
install -D mtCNNModels/* /usr/local/tartan/mtCNNModels/  -v
install /usr/local/tartan/facenetModels -d -v
install -D facenetModels/* /usr/local/tartan/facenetModels/  -v
install facenet.pb /usr/local/tartan/ -v
cp shinpark /var/ -rf
install tartan.service /lib/systemd/system/ -v
systemctl enable tartan 
systemctl start tartan 


