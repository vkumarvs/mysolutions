docker container run -d --name quotes alpine \
   /bin/sh -c "while :; do wget -qO- https://talaikis.com/api/quotes/random; printf '\n'; sleep 5; done"

#stop the container by name
docker container stop quotes

#stop the container by container ID
export CONTAINER_ID = $(docker container ls | grep quotes | awk '{print $1}')

docker container stop $CONTAINER_ID

#start the stopped container
docker container start quotes

#show all the containers that existed
docker container ls -a

#remove unused containers
docker container rm <container ID>
docker container rm <container name>

#forcefully remove a container even if it is running
docker container rm -f <container ID>

#inspecting docker container
docker container inspect <name>
#Filter inspect output, show only State
docker container inspect -f "{{json .State}}" quotes | jq

#Attach to a running container 
$ docker container attach <name> or <container ID>

#Run detach and in background a container
Ctrl+P Ctrl+Q

#We can even run processes as daemon using the flag -d and define environment variables using the -e flag variables as follows:
$ docker container exec -it \
    -e MY_VAR="Hello World" \
    quotes /bin/sh
# / echo $MY_VAR
Hello World
# / exit

#Run nginx on alpine linux distribution as daemon
docker run -d --name nginx -p 8080:80 nginx:alpine
#-d in daemon mode
#--name : container name
#-p: port

#Collect stdout/stderr logs of a docker container
docker container logs  <name or container ID>

docker container logs --tail 5 <container name or ID>

#equivalent to tailf command
docker container logs --tail 5 --follow <container name or id>

#Logging drivers to retrieve the logs of a container


#Mounting a volume
docker container run --name test -it \
     -v my-data:/data alpine /bin/sh

#removing the docker volume
docker volume rm my-data

#To remove all running containers to clean up the system, run the following command
docker container rm -f $(docker container ls -aq)


#decoker version info
docker version

docker system df
docker system df -v  (detailed info)

#Pruning containers
docker container prune 
docker container prune -f
docker container rm -f $(docker container ls -aq)

#pruning docker  images
docker image prune --force --all
docker image prune

#pruning docker volumes:
docker volume prune
docker volume prune --filter 'label=demo' --filter 'label=test'

#pruning networks
docker network prune

#pruning everything in one go:
docker system prune
#Docker system info
docker system info


