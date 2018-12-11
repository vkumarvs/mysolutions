mkdir -p /home/MSD/POCO_LEARNING/SIMPLE_HTTP_SERVER/USP-NGINX-MODULE/vip_nginx_install_1_12/conf
mkdir -p /home/MSD/POCO_LEARNING/SIMPLE_HTTP_SERVER/USP-NGINX-MODULE/vip_nginx_install_1_12/pid
mkdir -p /home/MSD/POCO_LEARNING/SIMPLE_HTTP_SERVER/USP-NGINX-MODULE/vip_nginx_install_1_12/lock
mkdir -p /home/MSD/POCO_LEARNING/SIMPLE_HTTP_SERVER/USP-NGINX-MODULE/vip_nginx_install_1_12/bin
mkdir -p /home/MSD/POCO_LEARNING/SIMPLE_HTTP_SERVER/USP-NGINX-MODULE/vip_nginx_install_1_12/logs

./configure --prefix=/home/MSD/POCO_LEARNING/SIMPLE_HTTP_SERVER/USP-NGINX-MODULE/vip_nginx_install_1_12 \
--sbin-path=/home/MSD/POCO_LEARNING/SIMPLE_HTTP_SERVER/USP-NGINX-MODULE/vip_nginx_install_1_12/bin \
--conf-path=/home/MSD/POCO_LEARNING/SIMPLE_HTTP_SERVER/USP-NGINX-MODULE/vip_nginx_install_1_12/conf/nginx.conf \
--pid-path=/home/MSD/POCO_LEARNING/SIMPLE_HTTP_SERVER/USP-NGINX-MODULE/vip_nginx_install_1_12/pid/nginx.pid \
--lock-path=/home/MSD/POCO_LEARNING/SIMPLE_HTTP_SERVER/USP-NGINX-MODULE/vip_nginx_install_1_12/lock/nginx.lock \
--error-log-path=/home/MSD/POCO_LEARNING/SIMPLE_HTTP_SERVER/USP-NGINX-MODULE/vip_nginx_install_1_12/logs/error.log \
--http-log-path=/home/MSD/POCO_LEARNING/SIMPLE_HTTP_SERVER/USP-NGINX-MODULE/vip_nginx_install_1_12/logs/access.log \
--with-debug \
--with-http_ssl_module \
--with-http_v2_module \
#--add-module=/home/MSD/POCO_LEARNING/SIMPLE_HTTP_SERVER/USP-NGINX-MODULE/vip_hello_module 
#--add-module=/home/MSD/POCO_LEARNING/SIMPLE_HTTP_SERVER/USP-NGINX-MODULE/nginx_mod_smooth_streaming-1.7.28
#--add-module=/home/MSD/POCO_LEARNING/SIMPLE_HTTP_SERVER/USP-NGINX-MODULE/nginx_mod_smooth_streaming-1.7.25

