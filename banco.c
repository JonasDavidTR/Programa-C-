#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // Se tiver no Linux/Mac sleep(segundos)
//#include <windows.h> //Se tiver no Windows Sleep(segundos)
#include <time.h>

typedef struct {
	int codigo;
	char nome[50];
	char email[50];
	char cpf[20];
	char dataNascimento[20];
	char dataCadastro[20];
} Cliente;

typedef struct {
	int numero;
	Cliente cliente;
	float saldo;
	float limite;
	float saldoTotal; //saldo + limite
} Conta;

	//Protótipos de Função
void menu();
void infoCliente(Cliente cliente);
void infoConta(Conta conta);
void criarConta();
void efetuarSaque();
void efetuarDeposito();
void efetuarTransferencia();
void listarContas();
float atualizaSaldoTotal(Conta conta);
Conta buscarContaPorNumero(int numero);
void sacar(Conta conta, float valor);
void depositar(Conta conta, float valor);
void transferir(Conta conta_origem, Conta conta_destino, float valor);

static Conta contas[50];
static int contador_contas = 0;
static int contador_clientes = 0;

int main(){
	menu();
	return 0;
}

void menu(){
	int opcao = 0;
	printf("======================================\n");
	printf("=============== ATM ==================\n");
	printf("==============  bank  ================\n");
	printf("======================================\n");

	printf("Selecione uma opção no menu:\n");
	printf("1 - Criar conta\n");
	printf("2 - Efetuar saque\n");
	printf("3 - Efetuar depósito\n");
	printf("4 - Efetuar transferência\n");
	printf("5 - Listar contas\n");
	printf("6 - Sair do sistema\n");

	scanf("%d", &opcao);
	getchar();

	switch(opcao){
		case 1:
			criarConta();
			break;
		case 2:
			efetuarSaque();
			break;
		case 3:
			efetuarDeposito();
			break;
		case 4:
			efetuarTransferencia();
			break;
		case 5:
			listarContas();
			break;
		case 6:
			printf("Até a próxima!\n");
			sleep(2); //Sleep(2)
			exit(0);
		default:
			printf("Opção inválida.\n");
			sleep(2);
			menu();
			break;
	}
}

//Funções
void infoCliente(Cliente cliente){
	printf("Código: %d \nNome: %s \nData de Nascimento: %s \nCadastro: %s\n",
			cliente.codigo, strtok(cliente.nome, "\n"), strtok(cliente.dataNascimento, "\n"),
			strtok(cliente.dataCadastro, "\n"));
}

void infoConta(Conta conta){
	printf("Número da conta: %d \nCliente: %s \nData Nascimento: %s \nData Cadastro: %s \nSaldo Total: %.2f\n",
			conta.numero, strtok(conta.cliente.nome, "\n"), strtok(conta.cliente.dataNascimento, "\n"),
			strtok(conta.cliente.dataCadastro, "\n"), conta.saldoTotal);
}

void criarConta(){
	Cliente cliente;

	//Data de cadastro
	char dia[3]; //06\0
	char mes[3]; //08
	char ano[5]; //2020
	char data_cadastro[20];
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	//dia
	if(tm.tm_mday < 10){
		sprintf(dia, "0%d", tm.tm_mday);// 1, 2, 3, 4, 5, 6, 7, 8, 9 -> 01/08/2020
	}else{
		sprintf(dia, "%d", tm.tm_mday);
	}

	//mês
	if((tm.tm_mon + 1) < 10){ // 01/08/2020
		sprintf(mes, "0%d", tm.tm_mon + 1);
	}else{
		sprintf(mes, "%d", tm.tm_mon + 1);
	}

	//ano
	sprintf(ano, "%d", tm.tm_year + 1900);

	strcpy(data_cadastro, "");
	strcat(data_cadastro, dia); //22
	strcat(data_cadastro, "/"); //22/
	strcat(data_cadastro, mes); //22/08
	strcat(data_cadastro, "/"); //22/08/
	strcat(data_cadastro, ano); //22/08/2020
	strcat(data_cadastro, "\0");
	strcpy(cliente.dataCadastro, data_cadastro);

	//Criar o cliente
	printf("Informe os dados do cliente:\n");
	cliente.codigo = contador_clientes + 1;

	printf("Nome do cliente: \n");
	fgets(cliente.nome, 50, stdin);

	printf("E-mail do cliente: \n");
	fgets(cliente.email, 50, stdin);

	printf("CPF do cliente: \n");
	fgets(cliente.cpf, 20, stdin);

	printf("Data de nascimento do cliente: \n");
	fgets(cliente.dataNascimento, 20, stdin);

	contador_clientes++;

	//Criar a conta
	contas[contador_contas].numero = contador_contas + 1;
	contas[contador_contas].cliente = cliente;
	contas[contador_contas].saldo = 0.0;
	contas[contador_contas].limite = 0.0;
	contas[contador_contas].saldoTotal = atualizaSaldoTotal(contas[contador_contas]);

	printf("Conta criada com sucesso!\n");
	printf("\n");
	printf("Dados da conta criada: \n");
	printf("\n");
	infoConta(contas[contador_contas]);
	contador_contas++;

	sleep(4);
	menu();
}

float atualizaSaldoTotal(Conta conta){
	return conta.saldo + conta.limite;
}

Conta buscarContaPorNumero(int numero){
	Conta c;
	if(contador_contas > 0){
		for(int i = 0; i < contador_contas; i++){
			if(contas[i].numero == numero){
				c = contas[i];
			}
		}
	}
	return c;
}

void sacar(Conta conta, float valor){
	if(valor > 0 && conta.saldoTotal >= valor){
		for(int i = 0; i < contador_contas; i++){
			if(contas[i].numero == conta.numero){
				if(contas[i].saldo >= valor){
					contas[i].saldo = contas[i].saldo - valor;
					contas[i].saldoTotal = atualizaSaldoTotal(contas[i]);
					printf("Saque efetuado com sucesso!\n");
				}else{
					float restante = contas[i].saldo - valor;
					contas[i].limite = contas[i].limite + restante;
					contas[i].saldo = 0.0;
					contas[i].saldoTotal = atualizaSaldoTotal(contas[i]);
					printf("Saque efetuardo com sucesso!\n");
				}
			}
		}
	}else{
		printf("Saque não realizado. Tente novamente.\n");
	}
}

void depositar(Conta conta, float valor){
	if(valor > 0){
		for(int i = 0; i < contador_contas; i++){
			if(contas[i].numero == conta.numero){
				contas[i].saldo = contas[i].saldo + valor;
				contas[i].saldoTotal = atualizaSaldoTotal(contas[i]);
				printf("Depósito efetuado com sucesso!\n");
			}
		}
	}else{
		printf("Erro ao efetuar depósito. Tente novamente.\n");
	}
}

void transferir(Conta conta_origem, Conta conta_destino, float valor){
	if(valor > 0 && conta_origem.saldoTotal >= valor){
		for(int co = 0; co < contador_contas; co++){
			if(contas[co].numero == conta_origem.numero){
				for(int cd = 0; cd < contador_contas; cd++){
					if(contas[cd].numero == conta_destino.numero){
						if(contas[co].saldo >= valor){
							contas[co].saldo = contas[co].saldo - valor;
							contas[cd].saldo = contas[cd].saldo + valor;
							contas[co].saldoTotal = atualizaSaldoTotal(contas[co]);
							contas[cd].saldoTotal = atualizaSaldoTotal(contas[cd]);
							printf("Transferência realizada com sucesso!\n");
						}else{
							float restante = contas[co].saldo - valor;
							contas[co].limite = contas[co].limite + restante;
							contas[co].saldo = 0.0;
							contas[cd].saldo = contas[cd].saldo + valor;
							contas[co].saldoTotal = atualizaSaldoTotal(contas[co]);
							contas[cd].saldoTotal = atualizaSaldoTotal(contas[cd]);
							printf("Transferência realizada com sucesso!\n");
						}
					}
				}
			}
		}
	}else{
		printf("Transferência não realizada. Tente novamente.\n");
	}
}

void efetuarSaque(){
	if(contador_contas > 0){
		int numero;
		printf("Informe o número da conta: \n");
		scanf("%d", &numero);

		Conta conta = buscarContaPorNumero(numero);

		if(conta.numero == numero){
			float valor;
			printf("Informe o valor do saque: \n");
			scanf("%f", &valor);

			sacar(conta, valor);
		}else{
			printf("Não foi encontrada uma conta com o número %d\n", numero);
		}


	}else{
		printf("Ainda não existem contas para saque.\n");
	}
	sleep(2);
	menu();
}

void efetuarDeposito(){
	if(contador_contas > 0){
			int numero;
			printf("Informe o número da conta: \n");
			scanf("%d", &numero);

			Conta conta = buscarContaPorNumero(numero);

			if(conta.numero == numero){
				float valor;
				printf("Informe o valor do depósito: \n");
				scanf("%f", &valor);

				depositar(conta, valor);
			}else{
				printf("Não foi encontrada uma conta com o número %d\n", numero);
			}


		}else{
			printf("Ainda não existem contas para depósito.\n");
		}
		sleep(2);
		menu();
}

void efetuarTransferencia(){
	if(contador_contas > 0){
		int numero_o, numero_d;
		printf("Informe o número da sua conta: \n");
		scanf("%d", &numero_o);

		Conta conta_o = buscarContaPorNumero(numero_o);

		if(conta_o.numero == numero_o){
			printf("Informe o número da conta destino: \n");
			scanf("%d", &numero_d);

			Conta conta_d = buscarContaPorNumero(numero_d);

			if(conta_d.numero == numero_d){
				float valor;
				printf("Informe o valor para transferência: \n");
				scanf("%f", &valor);

				transferir(conta_o, conta_d, valor);
			}else{
				printf("A conta destino com número %d não foi encontrada.\n", numero_d);
			}
		}else{
			printf("A conta com número %d não foi encontrada.\n", numero_o);
		}

	}else{
		printf("Ainda não existem contas para transferência.\n");
	}
	sleep(2);
	menu();
}

void listarContas(){
	if(contador_contas > 0){
		for(int i = 0; i < contador_contas; i++){
			infoConta(contas[i]);
			printf("\n");
			sleep(1);
		}
	}else{
		printf("Não existem contas cadastradas ainda.\n");
	}
	sleep(2);
	menu();
}

