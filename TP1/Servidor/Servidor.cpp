/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Servidor.cpp
 * Author: gustavo
 * 
 * Created on September 16, 2017, 12:12 PM
 */

#include "Servidor.h"

Servidor::Servidor(int porta) {
    this->porta = porta;
    
    // Inicializa array de clientes
    for(int i=0; i<MAX_CLIENTES; i++){
        this->clientes[i] = -1;
    }
}

Servidor::Servidor(const Servidor& orig) {
}

Servidor::~Servidor() {
}

void Servidor::iniciar(){
    struct sockaddr_in serv_addr;
    
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serv_addr.sin_port = htons(porta);
    
    socklen_t * addrlen;
    
    conexao = socket(AF_INET, SOCK_STREAM, 0);
    if(bind(conexao, (struct sockaddr *) &serv_addr, sizeof(serv_addr) < 0)){
        perror("Ocorreu um erro na conexão - bind");
        ::exit(1);
    }
    
    if(listen(conexao, MAX_CLIENTES) != 0){
        perror("Ocorreu um erro na conexão - listen");
        ::exit(1);
    }
}

int Servidor::aceitarCliente(){
    struct sockaddr * addr;
    socklen_t * addrlen;
        
    int cliente = accept(conexao,(struct sockaddr *) &addr, addrlen);
    
    insereCliente(cliente);
    
    return cliente;
}

void Servidor::encerrar(){
    ::close(conexao);
}

int Servidor::enviar(int maquina, Mensagem* mensagem){
    int numBytes = 0;
    char msg[255]; 
    mensagem->toChar(msg);
    if(clientes[maquina] != -1)
        numBytes = ::send(clientes[maquina], msg, strlen(msg), 0);
    
    return numBytes;
}

Mensagem* Servidor::receber(int maquina){
    int numBytes = 0;
    char msg[255]; 
    if(clientes[maquina] != -1)
        numBytes = ::recv(clientes[maquina], msg, 255, 0);
    
    if(numBytes > 0)
        return new Mensagem(msg);
    else
        return NULL;    
}

int Servidor::insereCliente(int cliente){
    int indice = proximoCliente();
    
    if(indice != -1){
        clientes[indice] = cliente;
    }
    
    return indice;
}

void Servidor::removeCliente(int i){
    if(i >= 0 && i < MAX_CLIENTES)
        clientes[i] = -1;
}

int Servidor::proximoCliente(){
    for(int i=0; i<MAX_CLIENTES; i++){
        if(clientes[i] == -1){
            return i;
        }
    }
    
    return -1;
}