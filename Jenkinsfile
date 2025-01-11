pipeline {
    agent any
    stages {
        stage('构建初始化') {
            steps {
                script {
                    // 使用三引号包裹多行 Shell 脚本
                    sh '''
                    cat <<EOF > ./Dockerfile
FROM ubuntu:20.04

RUN apt-get update && apt-get install -y build-essential wget git libboost-all-dev cmake curl vim net-tools && apt-get clean

WORKDIR /app

CMD ["/bin/bash"]
EOF

                    cat ./Dockerfile
                    '''
                }
            }
        }
        stage('构建') {
            steps {
                script {
                    // 明确上下文路径（当前目录 .）
                    sh '''
                    docker build -t l1akr-boost-env:v1.0 -f ./Dockerfile .
                    '''
                }
            }
        }
    }
}
