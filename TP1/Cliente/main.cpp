/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: gustavo
 *
 * Created on September 16, 2017, 4:09 PM
 */

#include <cstdlib>
#include <unistd.h>

#include "Cliente.h"
#include "Mensagem.h"
#include <fstream>
#include <iostream>

#define NOME_ARQUIVO "maquina.log"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    if(argc < 3){
        printf("Parâmetros insuficientes: entre com o ip e a porta.\n");
        printf("%s <IP_SERVIDOR> <PORTA_SERVIDOR>", argv[0]);
        //printf("Exemplo: %s 192.168.10.1 8000", argv[0]);
        return 1;
    }

    char ip_servidor[255];
    strcpy(ip_servidor, argv[1]);
    int porta_servidor = atoi(argv[2]);
    int porta_aplicacao = atoi(argv[3]);
    Cliente* cliente = new Cliente(ip_servidor, porta_servidor, porta_aplicacao);

    printf("Conectar ao servidor...");
    fflush(stdout);
    cliente->conectarAoServidor();
    printf("Ok\n");
    fflush(stdout);

    printf("Iniciar serviço background...");
    fflush(stdout);
    cliente->iniciarGrep();
    printf("Ok\n");
    fflush(stdout);


    int conexaoGrep = -1;
    
    int parent_pid = getpid();

    if (fork() < 0) {
        perror("Ocorreu um erro criando nova instância no servidor - fork");
    }

    while (true) {
            Mensagem* m = new Mensagem();
            if (getpid() == parent_pid){
                
                printf("\nAceitar.");
                fflush(stdout);
                conexaoGrep = cliente->aceitarGrep();
                printf("Ok - Cliente %d\n", conexaoGrep);
                fflush(stdout);

                //--------------------------------------------
                //Receber mensagem de solicitação de grep distribuído - 0

                printf("\nReceber.pid=%d ",getpid());
                fflush(stdout);
                m = cliente->receberDoGrep();
                printf("Ok\n");
                fflush(stdout);

                char msg[255];
                m->toChar(msg); ///
                printf("Mensagem: %s", msg);
                fflush(stdout);

                //--------------------------------------------
                //Enviar mensagem ao servidor solicitando grep distribuído - 1
                printf("\nEnviar.");
                fflush(stdout);
                m->setCodigo(1);
                m->toChar(msg); ///
                printf("Mensagem: %s", msg);
                cliente->enviarAoServidor(m);
                printf("Ok\n");
                fflush(stdout);

                //TEM Q PARAR AQUI ATE O OUTRO PROCESSO RECEBER E ENVIAR
                // if(conexaoGrep < 0)
                //     return 0;
                
                //--------------------------------------------
                //Receber mensagem do servidor solicitando grep local - 2
                printf("\nReceber...pid=%d ",getpid());
                fflush(stdout);
                m = cliente->receberDoServidor();
                m->toChar(msg); ///
                printf("Mensagem: %s", msg);
                printf("Ok hahahahaah\n");
                fflush(stdout);

                //--------------------------------------------
                //Executar grep local
                // printf("1\n");
                string comando = m->getTexto() + " " + NOME_ARQUIVO + " > log.temp";
                // printf("2\n");
                // system(comando.data());
                system("./simple.sh");
                // printf("3\n");
                std::ifstream arq("log.temp");
                // printf("4\n");
                string buffer;
                // printf("5\n");
                string result;
                // printf("6\n");
                while (getline(arq, buffer)) {
                    result += buffer;
                }
                // string result = "blalala";

                //--------------------------------------------
                //Enviar mensagem de resposta da solicitação de grep local - 3 [Resposta de 2]
                printf("\nEnviar...");
                fflush(stdout);
                m = new Mensagem(3, result.data());
                cliente->enviarAoServidor(m);
                printf("Ok\n");
                m->toChar(msg); ///
                printf("Mensagem: %s", msg);
                fflush(stdout);

                //--------------------------------------------
                //Receber mensagem do servidor com resposta de grep distribuído - 4 [Resposta de 1]
                printf("\nReceber.ansdasbdhiasbdiabdhba");
                fflush(stdout);
                m = cliente->receberDoServidor();
                m->toChar(msg);
                printf("Mensagem: %s", msg);
                printf("Okkkkk\n");
                fflush(stdout);

                //--------------------------------------------
                //Enviar mensagem à aplicação com resposta de grep distribuído - 5 [Resposta de 0]
                printf("\nEnviar.");
                fflush(stdout);
                m->setCodigo(5);
                m->toChar(msg); ///
                printf("Mensagem: %s", msg);
                cliente->enviarAoGrep(m);
                printf("Ok\n");
                fflush(stdout);
                cliente->encerrarGrep();
            }
            else {
                char msg[255];
                    
                // if(conexaoGrep >= 0)
                //     return 0;
                
                //--------------------------------------------
                //Receber mensagem do servidor solicitando grep local - 2
                printf("\nReceber...pid=%d ",getpid());
                fflush(stdout);
                m = cliente->receberDoServidor();
                m->toChar(msg); ///
                printf("Mensagem: %s", msg);
                printf("Ok hahahahaah\n");
                fflush(stdout);

                //--------------------------------------------
                //Executar grep local
                // printf("1\n");
                string comando = m->getTexto() + " " + NOME_ARQUIVO + " > log.temp";
                // printf("2\n");
                // system(comando.data());
                system("./simple.sh");
                // printf("3\n");
                std::ifstream arq("log.temp");
                // printf("4\n");
                string buffer;
                // printf("5\n");
                string result;
                // printf("6\n");
                while (getline(arq, buffer)) {
                    result += buffer;
                }
                // string result = "blalala";

                //--------------------------------------------
                //Enviar mensagem de resposta da solicitação de grep local - 3 [Resposta de 2]
                printf("\nEnviar...");
                fflush(stdout);
                m = new Mensagem(3, result.data());
                cliente->enviarAoServidor(m);
                printf("Ok\n");
                m->toChar(msg); ///
                printf("Mensagem: %s", msg);
                fflush(stdout);

                // // //--------------------------------------------
                // // //Receber mensagem do servidor com resposta de grep distribuído - 4 [Resposta de 1]
                // // printf("\nReceber...");
                // // fflush(stdout);
                // // m = cliente->receberDoServidor();
                // // printf("Ok\n");
                // // fflush(stdout);

                // // //--------------------------------------------
                // // //Enviar mensagem à aplicação com resposta de grep distribuído - 5 [Resposta de 0]
                // // printf("\nEnviar...");
                // // fflush(stdout);
                // // m->setCodigo(5);
                // // cliente->enviarAoGrep(m);
                // // printf("Ok\n");
                // // fflush(stdout);

                // // cliente->encerrarGrep();

            }
        
    }
    


    //cliente->encerrarServidor();

    //delete cliente;    

    return 0;
}

