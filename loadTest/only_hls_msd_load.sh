#g++ Mp4Dump.cpp poco_http_server.cpp -o pocoserver -L /home/vipin/VIEW/performance_main/lib/ -I. -I Bento4/include/ -L Bento4/lib/ -lAP4 -lPocoNet -lPocoUtil -lPocoFoundation -lfmp4
#g++ -std=c++0x msd_load.cpp -L /home/MSD/POCO_LEARNING/poco_install/lib -L /usr/lib64/ -I /home/MSD/POCO_LEARNING/poco_install/include -I. -lPocoNet -lPocoNetSSL -lPocoCrypto -lPocoUtil -lPocoFoundation 
g++ -std=c++11 only_hls_msd_load.cpp -L /home/MSD/POCO_LEARNING/poco_install/lib -L /usr/lib64/ -I /home/MSD/POCO_LEARNING/poco_install/include -I. -lPocoNet -lPocoNetSSL -lPocoCrypto -lPocoUtil -lPocoFoundation 
#g++ -std=c++0x msd_load.cpp -lPocoNet -lPocoNetSSL -lPocoCrypto -lPocoUtil -lPocoFoundation 
