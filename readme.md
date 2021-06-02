# Sobre

O Snake Game unix é uma adaptação do clássico jogo da cobrinha.
Ele é construido para rodar em terminais, testado no linux mas nada impede de compilar para outros sistemas.

# Como compilar

**Testado no Ubuntu  21.04, e 20.04.**
Você vai precisar instalar as dependências, que são o compilador e a biblioteca ncurses. 
Para instalar execute o seguinte comando seguinte comando:

    sudo apt install build-essential libncurses5-dev cmake

Faça download e na pasta raiz do projeto rode o cmake, ele vai criar o  arquivo Makefile em uma nova pasta chamada "build".

    cmake -S . -B build
Agora você ja tem tudo pronto , pode compilar e executar.

    cd build
    make && ./snake
o comando make vai compilar o projeto e criar um arquivo executavel com o nome "snake" dentro da pasta build.

