pipeline {
    agent any
    parameters {

    }
    stages {
        stage('构建初始化') {
            sh '''
            cat <<EOF > ./Dockerfile
            FROM ubuntu:20.04

            RUN apt-get update && apt-get install -y build-essential wget git libboost-all-dev cmake curl vim net-tools && apt-get clean

            WORKDIR /app

            CMD ["/bin/bash"]
            EOF
            '''
        }
        stage('构建') {
            sh '''
            docker build -t l1Akr-Boost_env:v1.0 -f ./Dockerfile
            '''
        }
    }
}