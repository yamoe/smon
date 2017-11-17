
# SMON
Self-Monitoring : simple monitoringn web server

## How to run
1. install java 8 (64bit)
1. download release
1. ./run_smon
1. open http://localhost:1980 in chrome, edge(not IE), firefox, opera

## How to Build
### Prerequisite
* install java 8 (used 1.8.0_151 64 bit)
* setting JAVA_HOME and PATH environment variable for Java
* install npm (used 3.10.10)
* install ng (angular/cli) : https://angular.io/tutorial/toh-pt0

### Build
> ./gradlew build

check 'deploy' directory

## Description
### Development environment
#### Windows
* WIndows 10
* Visual Studio 2017 (c++)
#### Ubuntu Linux (on docker)
* Ubuntu 16.04.2 LTS 64 bit (Linux version 4.9.49-moby)
* gcc version 5.4.0 20160609
* Dockerfile
```
FROM ubuntu:16.04

RUN apt-get update && apt-get install -y \
  vim build-essential git openjdk-8-jdk
ENV JAVA_HOME /usr/lib/jvm/java-8-openjdk-amd64
```
\> docker build -t dev_smon .
\> docker run -it -p 1980:1980 --name dev_smon dev_smon:latest


### Projects
* smon_front (web)
  * angular 4.2.4
  * ngx-charts 6.1.0
  * ng2-stompjs 0.4.2
  * vmware clarity 0.10.10
* smon_server (spring boot)
  * java 1.8.0_151 64bit
  * spring boot 1.5.7.RELEASE
* smon_native (os monitoring library)
  * JNI
  * c++(pdh)

### IDE
* Spring Tool Suite(STS) 3.9.1.RELEASE
* Visual Studio Code
* Wisual Studio 2017
* vim + YouCompleteMe


### Known Issue
1. Not working IE11 : ngx-charts (angular version problem)
1. Not working websocket auto reconnection : ng2-stompjs (underlying library problem)
