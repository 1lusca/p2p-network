## P2P Network

Implementação de uma rede p2p, para o Trabalho do Grau A, na cadeira de Redes de Computadores I.

Basicamente, o sistema cria 2 threads, para o sender e receiver dos dados.

Cada peer vai rodar o mesmo programa. Os peers são executados em containers.


## System

O sistema foi desenvolvimento em C, o ambiente foi um Linux Mint bare-metal, os peers rodam em containers Docker e o Vim foi utilizado como editor de texto.


## Getting started

### Instalar e rodar o Docker:

Vamos instalar o Docker no sistema e subir 3 containers rodando Ubuntu

1. ```sudo apt update```

2. ```sudo apt install apt-transport-https ca-certificates curl software-properties-common```

3. ```curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -```

4. ```sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu focal stable"```

5. ```sudo apt update```

6. ```apt-cache policy docker-ce```

7. ```docker search ubuntu```

8. ```docker pull ubuntu```

9. ```docker run -it ubuntu```


### Run

Vamos compilar o programa com o GCC e rodar

1. ```gcc -pthread -o main main.c```

2. ```./main```


### Usage

1. O sistema irá iniciar ouvindo para a chegada de pacotes
2. Usuário indica que deseja enviar um comando para os peers
3. Usuário informa os IP's dos peers da rede
4. Usuário digita o comando e o sistema envia para os peers
5. Os peers recebebem os dados e executam o comando no seu sistema


## Versioning

1.0.0


## Authors

* **LUCAS SCHNEIDER**: @1lusca (https://github.com/1lusca)
