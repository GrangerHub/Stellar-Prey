pipeline {
    agent { dockerfile true }

    stages {
        stage('Init') {
            steps {
                sh 'git submodule update --init --recursive'
                sh './sp-tools.sh init'
                sh './sp-tools.sh install_default_paks'
            }
        }
        stage('Config') {
            steps {
                sh './sp-tools.sh configure_cmake both'
            }
        }
        stage('Build') {
            steps {
                sh './sp-tools.sh build both'
            }
        }
    }
}

