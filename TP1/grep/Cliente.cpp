/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Cliente.cpp
 * Author: gustavo
 * 
 * Created on September 16, 2017, 9:13 PM
 */

#include <netinet/in.h>

#include "Cliente.h"

Cliente::Cliente(int porta) {
    this->porta = porta;
}

Cliente::Cliente(const Cliente& orig) {
}

Cliente::~Cliente() {
}

void Cliente::conectar(){
    struct sockaddr_in serv_addr;
    socklen_t * addrlen;
    
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_addr.s_addr = INADDR_ANY; 
    serv_addr.sin_port = htons(porta);
    
    conexao = socket(AF_INET, SOCK_STREAM, 0);
    if(connect(conexao, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        perror("Ocorreu um erro na conexão - connect");
        exit(1);
    }
}

void Cliente::encerrar(){
    close(conexao);
}

void Cliente::enviar(){
    
}

void Cliente::receber(){
    
}