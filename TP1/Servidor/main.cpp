/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: gustavo
 *
 * Created on September 16, 2017, 12:09 PM
 */

#include <cstdlib>
#include <unistd.h>

#include "Servidor.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    if (argc < 2) {
        printf("Parâmetros insuficientes: entre com a porta do servidor.\n");
        printf("%s <PORTA>\n", argv[0]);
        printf("Exemplo: %s 8000\n", argv[0]);
        //return 1;
    }

    int porta = atoi(argv[1]);

    Servidor* servidor = new Servidor(porta);

    printf("Iniciando servidor na porta %d...", porta);
    fflush(stdout);

    servidor->iniciar();

    printf("Ok\n");
    fflush(stdout);

    int filhoNovaConexao = 0;
    int filhoAceitaRecebe = 0;

    int indice_cliente = -1;
    int parent_pid = getpid();
    while (true) {
        printf("PARENT:%d",parent_pid);

        // if ((filhoAceitaRecebe = fork()) < 0) {
        //     perror("Ocorreu um erro ao definir se servidor deve aceitar conexão ou receber mensagem. - fork");
        // } else {

            // Aceita nova conexão
            //if (filhoAceitaRecebe != 0) {
            if (getpid() == parent_pid){
                printf("\nAguardando cliente...Pid=%d",getpid());
                fflush(stdout);

                indice_cliente = servidor->aceitarCliente();

                // printf("Pressione qualquer tecla para continuar...");
                // getchar();

                if (indice_cliente >= 0) {

                    printf("\nCliente conectado...Ok..indice=%d",indice_cliente);
                    fflush(stdout);

                    if ((filhoNovaConexao = fork()) < 0) {
                        perror("Ocorreu um erro criando nova instância no servidor - fork");
                    } else {
                        // Processo filho
                        if (filhoNovaConexao == 0) {
                            // servidor->encerrarServidor();
                            servidor->setConexao(indice_cliente);
                        }
                    }
                } else {
                    perror("Não foi possível realizar conexão com o cliente. - aceitarCliente");
                }

                // servidor->encerrarCliente();
            }// Recebe nova mensagem
            else {
                if(indice_cliente < 0)
                    return 0;
                
                //--------------------------------------------
                //Receber mensagem de solicitação de grep distribuído - 1
                printf("\nReceber...Pid=%d",getpid());
                printf("\nIndice cliente = %d",indice_cliente);
                fflush(stdout);
                Mensagem* m = servidor->receber(indice_cliente);
                char msg[255];
                m->toChar(msg); ///
                printf("Mensagem: %s", msg);
                printf("Ok\n");
                fflush(stdout);

                //--------------------------------------------
                //Enviar mensagem de solicitação de grep - 2                
                printf("\nEnviar...");
                fflush(stdout);
                m->setCodigo(2);
                // servidor->enviarTodos(m);
                servidor->enviar(indice_cliente, m);
                m->toChar(msg); ///
                printf("Mensagem: %s", msg);
                printf("Ok\n");
                fflush(stdout);

                //--------------------------------------------
                //Receber mensagem de resposta de solicitação de grep - 3 [Resposta de 2]                
                printf("\nReceber");
                fflush(stdout);
                vector<Mensagem*> mensagens = servidor->receberTodos();
                mensagens.at(0)->toChar(msg);
                printf("%s", msg);
                printf("Ok\n");
                fflush(stdout);

                //--------------------------------------------
                //Agrupa conteúdos das mensagens
                m = servidor->agruparMensagens(mensagens);
                m->toChar(msg); ///
                printf("Mensagem: %s", msg);

                //--------------------------------------------
                //Enviar mensagem de resposta de solicitação de grep distribuído - 4 [Resposta de 1]
                printf("\nEnviar");
                fflush(stdout);
                m->setCodigo(4);
                servidor->enviar(indice_cliente, m);
                printf("Ok\n");
                fflush(stdout);
            }
        // }


    }
    return 0;
}

