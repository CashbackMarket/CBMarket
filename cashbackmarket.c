#ifdef __WIN32
#define limpar_tela "cls"
#else
#define limpar_tela "clear"
#endif
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// quantidade máxima de letras no nome do cliente e nome do produto
#define MAX 100
 
// definição da struct produto
typedef struct produto
{
	int id;
	int id_cliente;
	char nome[MAX];
	float preco;
} t_produto;
 
//definição da struct cliente
typedef struct cliente
{
	int id;
	char nome[MAX];
	char endereco[MAX];
	char telefone[MAX];
} t_cliente;
 
 
//FUNCOES
 
char menu();
void cadastrar_cliente();
void cadastrar_produto();
void listar_produtos();
void comprar_produto();
void listar_clientes();
t_produto *obter_produto(FILE *arq_produtos, int id_produto);
t_cliente *obter_cliente(FILE *arq_clientes, int id_cliente);
void pesquisar_produto();
void pagar_produto();
void bonus();
void pesquisar_cliente();
int existe_produto(FILE *arq_produtos, int id_produto);
int existe_cliente(FILE *arq_clientes, int id_cliente);
void excluir_produto();
void excluir_cliente();
 int str_somente_numeros(char str[]);
  
int main(int argc, char *argv[])
{
	char resp;
 
	// loop infinito do programa
	while(1)
	{
		// obtém a escolha do usuário
		resp = menu();
 
		// testa o valor de "resp"
		if(resp == '1')
			cadastrar_produto();
		else if(resp == '2')
			listar_produtos();
		else if(resp == '3')
			pesquisar_produto();
		else if(resp == '4')
			excluir_produto();
		else if(resp == '5')
			cadastrar_cliente();
		else if(resp == '6')
			listar_clientes();
		else if(resp == '7')
			pesquisar_cliente();
    else if(resp == '8')
      excluir_cliente();
    else if(resp == 'A' || resp == 'a')
			comprar_produto();
		else if(resp == 'b' || resp == 'B')
			 bonus();
		else if(resp == '0') // se for igual a 0, então sai do loop while
			break;
		else
		{
			printf("\nOpcao invalida! Pressione  | Ebnter |  para continuar...");
			scanf("%*c");
			// uma forma de "limpar" o buffer de entrada
			fseek(stdin, 0, SEEK_END); 
		}
		system(limpar_tela);
	}
 
	return 0;
}
 
 
// função que exibe o menu e retorna a opção escolhida pelo usuário
char menu()
{
	char resp[2];
 
	printf("------------------------- Cashback Market -----------------------");
	printf("\n\n---------------------------- Produtos ---------------------------\n\n");
	printf("| 1 | - Cadastrar um produto\n");
	printf("| 2 | - Listar todos os produtos\n");
	printf("| 3 | - Pesquisar por produto\n");
	printf("| 4 | - Excluir um produto\n");
	printf("\n---------------------------- Clientes ---------------------------\n\n");
	printf("| 5 | - Cadastrar um cliente\n");
	printf("| 6 | - Listar todos os clientes\n");
	printf("| 7 | - Pesquisar por cliente\n");
  printf("| 8 | - Excluir um cliente\n");
	printf("\n---------------------- Compras & Cashback -----------------------\n\n");
	printf("| A | - Realizar compra\n");
	printf("| B | - Bonificar Clientes\n");
	printf("\n------------------------------ FIM ------------------------------\n\n");
	printf("| 0 | - Sair\n");
	printf("Digite o numero da opcao: ");
	scanf("%1s%*c", resp); // o *c pega o Enter e descarta
 
	// uma forma de "limpar" o buffer de entrada
	fseek(stdin, 0, SEEK_END); 
 
	// se chegou aqui, é porque a opção é válida
	return resp[0];
}
 
 
// função que verifica se uma string contém somente números
int str_somente_numeros(char str[])
{
	int i = 0;
	int len_str = strlen(str);
 
	for(i = 0; i < len_str; i++)
	{
		if(str[i] < '0' || str[i] > '9')
			return 0;
	}
	return 1;
}
 
// função para cadastrar cliente
void cadastrar_cliente()
{
	// abre o arquivo para escrita
	// a+b => acrescenta dados ao final do arquivo ou cria
	// um arquivo binária para leitura e escrita
	FILE *arq_clientes = fopen("clientes.bin", "a+b");
 
	// testa a abertura do arquivo
	if(arq_clientes == NULL)
	{
		printf("\nFalha ao abrir arquivo(s)!\n");
		exit(1); // aborta o programa
	}
	/*
		NÃO é interessante que o usuário digite o ID do cliente, esse
		ID tem que ser gerado automático, então temos que pegar o
		ID do último usuário cadastrado
	*/
	int cont_bytes = 0;
 
	// cont irá guardar o número total de bytes
	// seta o ponteiro do arquivo para o final do arquivo
	fseek(arq_clientes, 0, SEEK_END);
	// pegamos a quantidade de bytes com a função ftell
	cont_bytes = ftell(arq_clientes);
 
	t_cliente cliente;
 
	if(cont_bytes == 0)
	{
		// se for 0, então não existe cliente cadastrado
		// coloco o ID começando de 1
		cliente.id = 1;
	}
	else
	{
		t_cliente ultimo_cliente;
 
		// utilizo a função fseek para posicionar o arquivo
		// cont_bytes - sizeof(t_cliente) serve para posicionar
		// para que possamos pegar o último cliente cadastrado
		fseek(arq_clientes, cont_bytes - sizeof(t_cliente), SEEK_SET);
 
		// ler o cliente
		fread(&ultimo_cliente, sizeof(t_cliente), 1, arq_clientes);
 
		// o ID do cliente é o ID do último cliente acrescido em 1
		cliente.id = ultimo_cliente.id + 1;
	}
	// ^\n indica para pegar até a quebra de linha (enter)
	// %*c descarta o enter
	printf("\nDigite o nome do cliente: ");
	scanf("%99[^\n]%*c", cliente.nome);

	printf("\nDigite o endereco: ");
	scanf("%99[^\n]%*c", cliente.endereco);

	printf("\nDigite o telefone: ");
	scanf("%99[^\n]%*c", cliente.telefone);
 
	// uma forma de "limpar" o buffer de entrada
	fseek(stdin, 0, SEEK_END); 
 
	// se o ponteiro não estiver no final do arquivo nada é escrito
	fseek(arq_clientes, 0, SEEK_END);
	// escreve no arquivo
	fwrite(&cliente, sizeof(t_cliente), 1, arq_clientes);
 
	// fecha o arquivo
	fclose(arq_clientes);
 
	printf("\nCliente \"%s\" cadastrado com sucesso!\n", cliente.nome);
	printf("\nPressione <Enter> para continuar...");
	scanf("%*c"); // pega o Enter e descarta
 
	// uma forma de "limpar" o buffer de entrada
	fseek(stdin, 0, SEEK_END); 
}
 
 
// função para cadastrar produto
void cadastrar_produto()
{
	// abre o arquivo para escrita
	// a+b => acrescenta dados ao final do arquivo ou cria
	// um arquivo binária para leitura e escrita
	FILE *arq_produtos = fopen("produtos.bin", "a+b");
 
	// testa a abertura do arquivo
	if(arq_produtos == NULL)
	{
		printf("\nFalha ao abrir arquivo(s)!\n");
		exit(1); // aborta o programa
	}
	t_produto produto;
 
	/*
		NÃO é interessante que o usuário digite o ID do produto, esse
		ID tem que ser gerado automático, então temos que pegar o
		ID do último produto cadastrado
	*/
	int cont_bytes = 0;
 
	// seta o ponteiro do arquivo para o final do arquivo
	fseek(arq_produtos, 0, SEEK_END);
	// cont irá guardar o número total de bytes
	cont_bytes = ftell(arq_produtos);
 
	if(cont_bytes == 0)
	{
		// se for 0, então não existe produto cadastrado
		// coloco o ID começando de 1
		produto.id = 1;
	}
	else
	{
		t_produto ultimo_produto;
 
		// utilizo a função fseek para posicionar o arquivo
		// cont_bytes - sizeof(t_cliente) serve para posicionar
		// para que possamos pegar o último produto cadastrado
		fseek(arq_produtos, cont_bytes - sizeof(t_produto), SEEK_SET);
 
		// ler o produto
		fread(&ultimo_produto, sizeof(t_produto), 1, arq_produtos);
 
		// o ID do produto é o ID do último produto acrescido em 1
		produto.id = ultimo_produto.id + 1;
	}
	// obtém o nome do produto
	// ^\n indica para pegar até a quebra de linha (enter)
	// %*c descarta o enter
	printf("\nDigite o nome do produto: ");
	scanf("%99[^\n]%*c", produto.nome);
 
	// uma forma de "limpar" o buffer de entrada
	fseek(stdin, 0, SEEK_END); 
 
	do
	{
		char str_preco[5];
		float float_preco;
		int somente_numeros = 1;
 
		// obtém o preço do produto
		printf("Digite o preco do produto: ");
		scanf("%10s%*c", str_preco);
 
		fseek(stdin, 0, SEEK_END); 
 
		// verifica se o preço possui somente números
		somente_numeros = str_somente_numeros(str_preco);
		// verifica se o preço contém somente números
		if(somente_numeros == 1)
		{
			// se chegou aqui, é porque tudo está validado
			// preencho a variável float_preco com o valor de str_preco
			int int_preco;
			// exemplo: 49,50 deve digitar 4950
			sscanf(str_preco, "%d", &int_preco);
			float_preco = int_preco / 100.0;
			produto.preco = float_preco;
			break;
		}
	}
	while(1);
 
	produto.id_cliente = -1;
 
	// se o ponteiro não estiver no final do arquivo nada é escrito
	fseek(arq_produtos, 0, SEEK_END);
	// escreve no arquivo
	fwrite(&produto, sizeof(t_produto), 1, arq_produtos);
 
	// fecha o arquivo
	fclose(arq_produtos);
 
	printf("\nproduto \"%s\" cadastrado com sucesso!\n", produto.nome);
	printf("\nPressione <Enter> para continuar...");
	scanf("%*c"); // pega o Enter e descarta
 
	// uma forma de "limpar" o buffer de entrada
	fseek(stdin, 0, SEEK_END); 
}
 
 
// função para listar todos os clientes
void listar_clientes()
{
	// rb => abre arquivo binário para leitura apenas
	FILE *arq_clientes = fopen("clientes.bin", "rb");
 
	// testa a abertura do arquivo
	if(arq_clientes == NULL)
	{
		printf("\nFalha ao abrir arquivo(s) ou ");
		printf("Nenhum cliente cadastrado.\n");
		printf("\nPressione <Enter> para continuar...");
		scanf("%*c"); // pega o Enter e descarta
 
		// uma forma de "limpar" o buffer de entrada
		fseek(stdin, 0, SEEK_END); 
		return;
	}
 
	// variável que indica se encontrou pelo menos 1 cliente
	int encontrou_clientes = 0;
	t_cliente cliente;
 
	printf("\nListando todos os clientes...\n");
	// loop para percorrer o arquivo
	while(1)
	{
		// fread retorna o número de elementos lidos com sucesso
		size_t result = fread(&cliente, sizeof(t_cliente), 1, arq_clientes);
 
		// se for 0, é porque não há mais elemento, então sai do loop
		if(result == 0)
			break;
 
		// atualiza a variável indicando que encontrou
		// pelo menos um cliente
		encontrou_clientes = 1;
 
		// mostra os dados do cliente
		printf("\nID: %d\n", cliente.id);
		printf("Nome: %s\n", cliente.nome);
		printf("Telefone: %s\n", cliente.telefone);		
		printf("Endereco: %s\n", cliente.endereco);		
	}
 
	if(encontrou_clientes == 0)
		printf("\nNenhum cliente cadastrado.\n");
 
	fclose(arq_clientes);
 
	printf("\nPressione <Enter> para continuar...");
	scanf("%*c");
 
	// uma forma de "limpar" o buffer de entrada
	fseek(stdin, 0, SEEK_END); 
}
 
 
// função que obtém um cliente pelo ID
t_cliente *obter_cliente(FILE *arq_clientes, int id_cliente)
{
	// vai para o início do arquivo
	rewind(arq_clientes);
 
	t_cliente *cliente;
 
	// loop para percorrer o arquivo
	// busca linear O(n), como o ID é crescente é possível fazer uma busca binária O(log(n))
	// aloca espaço mesmo sem saber se o cliente existe
	cliente = (t_cliente *)malloc(sizeof(t_cliente));
	while(1)
	{
 
		// fread retorna o número de elementos lidos com sucesso
		size_t result = fread(cliente, sizeof(t_cliente), 1, arq_clientes);
 
		// se for 0, é porque não há mais elemento, então sai do loop
		if(result == 0)
		{
			free(cliente); // libera a mémoria, pois o cliente não foi encontrado
			return NULL;
		}
		// verifica se os ID's são iguais
		if(cliente->id == id_cliente)
			break;
	}
	return cliente;
}
 
 
// função para listar todos os produtos
void listar_produtos()
{
	// lista de todos os produtos
 
	// rb => abre para leitura somente, ponteiro para o início do arquivo
	FILE *arq_produtos = fopen("produtos.bin", "rb");
	FILE *arq_clientes = fopen("clientes.bin", "rb");
 
	// se o arquivo de produtos não existe
	if(arq_produtos == NULL)
	{
		printf("\nFalha ao abrir arquivo ou ");
		printf("Nenhum produto cadastrado.\n");
		printf("\nPressione <Enter> para continuar...");
		scanf("%*c"); // pega o Enter e descarta
 
		// uma forma de "limpar" o buffer de entrada
		fseek(stdin, 0, SEEK_END); 
		return;
	}
	// variável que indica se encontrou pelo menos 1 produto
	int encontrou_produtos = 0;
	printf("\nListando todos os produtos...\n");
	// loop para percorrer o arquivo
	t_produto produto;
	while(1)
	{
		// fread retorna o número de elementos lidos com sucesso
		size_t result = fread(&produto, sizeof(t_produto), 1, arq_produtos);
 
		// se for 0, é porque não há mais elemento, então sai do loop
		if(result == 0)
			break;
 
		// atualiza a variável indicando que encontrou
		// pelo menos um produto
		encontrou_produtos = 1;
 
		// mostra os dados do produto
		printf("\nID do produto: %d\n", produto.id);
		printf("Nome do produto: %s\n", produto.nome);
		printf("Preco: %.2f\n", produto.preco);
 
		if(produto.id_cliente != -1)
		{

			if(arq_clientes == NULL)
			{
				printf("\nFalha ao abrir arquivo!\n");
				fclose(arq_produtos); // libera recursos
				exit(1); // aborta o programa
			}
			t_cliente *cliente = obter_cliente(arq_clientes, produto.id_cliente);
			free(cliente); // evita vazamento de memória
		}
	}
 
	// verifica se encontrou pelo menos um produto
	if(encontrou_produtos == 0)
		printf("\nNenhum produto cadastrado.\n");
 
	// fecha os arquivos em ordem inversa que foram abertos
	if(arq_clientes != NULL)
		fclose(arq_clientes); // apenas se foi aberto
	fclose(arq_produtos);
 
	printf("\nPressione <Enter> para continuar...");
	scanf("%*c");
 
	// uma forma de "limpar" o buffer de entrada
	fseek(stdin, 0, SEEK_END); 
}
 
 
// função para pesquisar por algum produto
void pesquisar_produto()
{
	char nome[MAX];
	int encontrou_produto = 0;
 
	// rb => abre para leitura somente
	FILE *arq_produtos = fopen("produtos.bin", "rb");
	FILE *arq_clientes = fopen("clientes.bin", "rb");
 
	// testa a abertura do arquivo
	if(arq_produtos == NULL)
	{
		printf("\nFalha ao abrir arquivo(s)!\n");
		exit(1); // aborta o programa
	}
 
	printf("\nDigite o nome do produto: ");
	scanf("%99[^\n]%*c", nome);
 
	printf("\nprodutos com o nome \"%s\":\n\n", nome);
	// loop para percorrer o arquivo
	// busca linear, pois o campo nome não possui índice
	t_produto produto;
	while(1)
	{
		// fread retorna o número de elementos lidos com sucesso
		size_t result = fread(&produto, sizeof(t_produto), 1, arq_produtos);
 
		// se for 0, é porque não há mais elemento, então sai do loop
		if(result == 0)
			break;
 
		char nome_aux[MAX];
		// faz uma cópia para não alterar produto.nome
		strcpy(nome_aux, produto.nome);
 
		// verifica se é igual
		if(strcmp(strupr(nome_aux), strupr(nome)) == 0)
		{
			// mostra os dados do produto
			printf("ID do produto: %d\n", produto.id);
			printf("Preco: %.2lf\n", produto.preco);
			if(produto.id_cliente != -1)
			{
				// se o arquivo de clientes não existir
				if(arq_clientes == NULL)
				{
					printf("\nFalha ao abrir arquivo!\n");
					fclose(arq_produtos); // libera recursos
					exit(1); // aborta o programa
				}
				t_cliente *cliente = obter_cliente(arq_clientes, produto.id_cliente);
					free(cliente); // evita vazamento de memória
			}
			encontrou_produto = 1;
			printf("\n");
		}
	}
 
	if(encontrou_produto == 0)
		printf("Nenhum produto encontrado.\n\n");
 
	fclose(arq_produtos);
 
	printf("Pressione <Enter> para continuar...");
	scanf("%*c");
 
	// uma forma de "limpar" o buffer de entrada
	fseek(stdin, 0, SEEK_END); 
}
 
 
// função para pesquisar por algum cliente
void pesquisar_cliente()
{
	char nome[MAX];
	int encontrou_cliente = 0;
 
	// rb+ => abre para leitura somente
	FILE *arq_clientes = fopen("clientes.bin", "rb");
 
	// testa a abertura do arquivo
	if(arq_clientes == NULL)
	{
		printf("\nFalha ao abrir arquivo(s)!\n");
		exit(1); // aborta o programa
	}
 
	printf("\nDigite o nome do cliente: ");
	scanf("%99[^\n]%*c", nome);
 
	printf("\nClientes com o nome \"%s\":\n\n", nome);
	// loop para percorrer o arquivo
	t_cliente cliente;
	while(1)
	{
		// fread retorna o número de elementos lidos com sucesso
		size_t result = fread(&cliente, sizeof(t_cliente), 1, arq_clientes);
 
		// se for 0, é porque não há mais elemento, então sai do loop
		if(result == 0)
			break;
 
		char nome_aux[MAX];
		// faz uma cópia para não alterar cliente->nome
		strcpy(nome_aux, cliente.nome);
 
		// verifica se é igual
		if(strcmp(strupr(nome_aux), strupr(nome)) == 0)
		{
			// mostra os dados do cliente
			printf("ID do cliente: %d\n\n", cliente.id);
			encontrou_cliente = 1;
		}
	}
 
	if(encontrou_cliente == 0)
		printf("Nenhum cliente encontrado.\n\n");
 
	// fecha o arquivo
	fclose(arq_clientes);
 
	printf("Pressione <Enter> para continuar...");
	scanf("%*c");
 
	// uma forma de "limpar" o buffer de entrada
	fseek(stdin, 0, SEEK_END); 
}
 
 
// função que verifica se um produto existe
// retorna 0 se NÃO existe e 1 caso contrário
int existe_produto(FILE *arq_produtos, int id_produto)
{
	// vai para o início do arquivo, pois não sabemos a posição do ponteiro no arquivo
	rewind(arq_produtos);
 
	t_produto produto;
	// loop para percorrer o arquivo
	// busca linear O(n), como o ID é crescente é possível fazer uma busca binária O(log(n))
	while(1)
	{
 
		// fread retorna o número de elementos lidos com sucesso
		size_t result = fread(&produto, sizeof(t_produto), 1, arq_produtos);
 
		// se for 0, é porque não há mais elemento, então sai do loop
		if(result == 0)
			break;
 
		// verifica se o ID é igual
		if(produto.id == id_produto)
			return 1;
	}
 
	// se chegou aqui é porque NÃO existe o produto, então retorna 0
	return 0;
}
 
 
// função que verifica se um cliente existe
// retorna 0 se NÃO existe e 1 caso contrário
int existe_cliente(FILE *arq_clientes, int id_cliente)
{
	// vai para o início do arquivo
	rewind(arq_clientes);
 
	t_cliente cliente;
	// loop para percorrer o arquivo
	// busca linear O(n), como o ID é crescente é possível fazer uma busca binária O(log(n))
	while(1)
	{
		// fread retorna o número de elementos lidos com sucesso
		size_t result = fread(&cliente, sizeof(t_cliente), 1, arq_clientes);
 
		// se for 0, é porque não há mais elemento, então sai do loop
		if(result == 0)
			break;
 
		// verifica se o ID é igual
		if(cliente.id == id_cliente)
			return 1;
	}
 
	// se chegou aqui é porque NÃO existe o cliente, então retorna 0
	return 0;
}
 
 
// função que obtém um produto pelo ID
t_produto *obter_produto(FILE *arq_produtos, int id_produto)
{
	// vai para o início do arquivo
	rewind(arq_produtos);
 
	// loop para percorrer o arquivo
	// busca linear O(n), como o ID é crescente é possível fazer uma busca binária O(log(n))
	t_produto *produto;
 
	// aloca espaço mesmo sem saber se o produto existe
	produto = (t_produto *)malloc(sizeof(t_produto));
	while(1)
	{
 
		// fread retorna o número de elementos lidos com sucesso
		size_t result = fread(produto, sizeof(t_produto), 1, arq_produtos);
 
		// se for 0, é porque não há mais elemento, então sai do loop
		if(result == 0)
			break;
 
		// verifica se os ID's são iguais
		if(produto->id == id_produto)
			return produto;
	}
	free(produto); // libera recursos
	return NULL;
}
 
 
// função para atualizar um produto
// recebe o ponteiro para o arquivo e o produto
void atualizar_produtos(FILE *cashback, t_produto *produto_comprado)
{
	// vai para o início do arquivo
	rewind(cashback);
 
	t_produto produto;
	while(1) // loop para percorrer o arquivo
	{
 
		// fread retorna o número de elementos lidos com sucesso
		size_t result = fread(&produto, sizeof(t_produto), 1, cashback);
 
		// se for 0, é porque não há mais elemento, então sai do loop
		if(result == 0)
			break;
 
		if(produto.id == produto_comprado->id)
		{
			// fseek posiciona o arquivo
			fseek(cashback, - sizeof(t_produto), SEEK_CUR);
			// atualiza o produto
			fwrite(produto_comprado, sizeof(t_produto), 1, cashback);
			break; // sai do loop
		}
	}
}
 
// função responsável por excluir produtos
void excluir_produto()
{
	char str_id_produto[10];
	int id_produto;
 
	printf("\nDigite o ID do produto: ");
	scanf("%10s%*c", str_id_produto);
 
	fseek(stdin, 0, SEEK_END); 
 
	// verifica se str_id_produto só contém números
	if(str_somente_numeros(str_id_produto) == 1)
	{
		// se chegou aqui é porque o ID do produto é válido
		sscanf(str_id_produto, "%d", &id_produto);
 
		// rb abre para leitura (o arquivo deve existir)
		FILE *arq_produtos = fopen("produtos.bin", "rb");
 
		if(arq_produtos == NULL)
		{
			printf("\nFalha ao abrir arquivo(s)!\n");
			exit(1); // aborta o programa
		}
 
		// verifica se o produto existe
		if(existe_produto(arq_produtos, id_produto) == 1)
		{
			char nome_produto[MAX];
			// abre um novo arquivo temporário
			FILE *arq_temp = fopen("temp_produtos.bin", "a+b");
			if(arq_temp == NULL)
			{
				printf("\nFalha ao criar arquivo temporario!\n");
				fclose(arq_produtos);
				exit(1); // aborta o programa
			}
			rewind(arq_produtos); // vai para o início do arquivo
 
			t_produto produto;
			while(1) // loop para percorrer o arquivo
			{
 
				// fread retorna o número de elementos lidos com sucesso
				size_t result = fread(&produto, sizeof(t_produto), 1, arq_produtos);
 
				// se for 0, é porque não há mais elemento, então sai do loop
				if(result == 0)
					break;
 
				// só copia pro novo arquivo se for diferente
				if(produto.id != id_produto)
				{
					// escreve no arquivo temporário
					fwrite(&produto, sizeof(t_produto), 1, arq_temp);
				}
				else
					strcpy(nome_produto, produto.nome);
			}
 
			// antes de fazer operações de remover arquivo e renomear,
			// é preciso fechar os dois arquivos
			fclose(arq_produtos);
			fclose(arq_temp);
 
			// depois de fechar o arquivo, então tentamos remover
			if(remove("produtos.bin") != 0)
				printf("\nErro ao deletar o arquivo \"produtos.bin\"\n");
			else
			{
				// renomeia o arquivo
				int r = rename("temp_produtos.bin", "produtos.bin");
				if(r != 0)
				{
					printf("\nPermissao negada para renomear o arquivo!\n");
					printf("Feche esse programa bem como o arquivo \"temp_produtos.bin\" e renomeie manualmente para \"produtos.bin\"\n");
				}
				else
					printf("\nproduto \"%s\" removido com sucesso!\n", nome_produto);
			}
		}
		else
		{
			fclose(arq_produtos);
			printf("\nNao existe produto com o ID \"%d\".\n", id_produto);
		}
	}
	else
		printf("\nO ID so pode conter numeros!\n");
 
	printf("\nPressione <Enter> para continuar...");
	scanf("%*c");
	fseek(stdin, 0, SEEK_END); 
}
void comprar_produto()
{
	char str_id_cliente[10];
	int id_cliente;
 
	// rb+ abre para leitura/atualização
	FILE *arq_produtos = fopen("produtos.bin", "rb+");
	FILE *arq_clientes = fopen("clientes.bin", "rb+");
 	FILE *cashback = fopen("cashback.bin", "rb+");
	// se não conseguiu abrir, então cria o arquivo
	// wb+ abre para escrita/atualização (cria o arquivo se ele NÃO existir)
	if(arq_produtos == NULL)
	{
		arq_produtos = fopen("produtos.bin", "wb+");
		if(arq_produtos == NULL)
		{
			printf("\nFalha ao criar arquivo(s)!\n");
			exit(1); // aborta o programa
		}
	}
 	if(cashback == NULL)
	{
		cashback = fopen("cashback.bin", "wb+");
		if(cashback == NULL)
		{
			printf("\nFalha ao criar arquivo(s)!\n");
			exit(1); // aborta o programa
		}
	}

	if(arq_clientes == NULL)
	{
		arq_clientes = fopen("clientes.bin", "wb+");
		if(arq_clientes == NULL)
		{
			printf("\nFalha ao criar arquivo(s)!\n");
			exit(1); // aborta o programa
		}
	}
 
	// obtém o ID do cliente
	printf("\nDigite o ID do cliente: ");
	scanf("%10s%*c", str_id_cliente);
 
	fseek(stdin, 0, SEEK_END); 
 
	if(str_somente_numeros(str_id_cliente) == 1)
	{
		// se caiu aqui é porque o ID possui somente números, então
		// preenche a variável "id_cliente" com o valor de "str_id_cliente"
		sscanf(str_id_cliente, "%d", &id_cliente);
		// verifica se o ID do do cliente existe
		if(existe_cliente(arq_clientes, id_cliente))
		{
			char str_id_produto[10];
			int id_produto;
 
			printf("\nDigite o ID do produto: ");
			scanf("%10s%*c", str_id_produto);
 
			fseek(stdin, 0, SEEK_END); 
 
			if(str_somente_numeros(str_id_produto) == 1)
			{
				// se chegou aqui é porque o ID do produto é válido
				sscanf(str_id_produto, "%d", &id_produto);
 
				// obtém o produto pelo ID
				t_produto *produto = obter_produto(arq_produtos, id_produto);
 
				// testa se o produto existe...
				if(produto != NULL)
				{
					// se chegou aqui é porque o produto existe
	
						// id_cliente do produto para associar o cliente
						produto->id_cliente = id_cliente;
						atualizar_produtos(arq_produtos, produto); // atualiza o produto no arquivo
						printf("\nproduto \"%s\" Comprado com sucesso no valor de R$ %.2lf !\n", produto->nome,produto->preco);
					
					free(produto); // libera o produto alocado
				}
				else
					printf("\nNao existe produto com o ID \"%d\".\n", id_produto);
			}
			else
				printf("\nO ID so pode conter numeros!\n");
		}
		else
			printf("\nNao existe cliente com o ID \"%d\".\n", id_cliente);
	}
	else
		printf("\nO ID so pode conter numeros!\n");
 
	// fecha os arquivos
	fclose(arq_clientes);
	fclose(arq_produtos);
	fclose(cashback);

 
	printf("\nPressione <Enter> para continuar...");
	scanf("%*c");
 
	fseek(stdin, 0, SEEK_END); 
}
void pagar_produto()
{
	char str_id_produto[10];
	int id_produto;
 
	// rb+ abre para leitura/atualização
	FILE *arq_produtos = fopen("produtos.bin", "rb+");
 
	// se não conseguiu abrir, então cria o arquivo
	// wb+ abre para escrita/atualização (cria o arquivo se ele NÃO existir)
	if(arq_produtos == NULL)
	{
		arq_produtos = fopen("produtos.bin", "wb+");
		if(arq_produtos == NULL)
		{
			printf("\nFalha ao criar arquivo(s)!\n");
			exit(1); // aborta o programa
		}
	}
 
	printf("\nDigite o ID do produto: ");
	scanf("%10s%*c", str_id_produto);
 
	fseek(stdin, 0, SEEK_END); 
 
	if(str_somente_numeros(str_id_produto) == 1)
	{
		// se chegou aqui é porque o ID do produto é válido
		sscanf(str_id_produto, "%d", &id_produto);
 
		// obtém o produto pelo ID
		t_produto *produto = obter_produto(arq_produtos, id_produto);
 
		// testa se o produto existe...
		if(produto != NULL)
		{
			// se chegou aqui é porque o produto existe
			// testa se o produto já foi entregue
			if(produto->id_cliente == -1)
				printf("\nO produto \"%s\" ja esta disponivel!\n", produto->nome);
			else
			{
				// se o produto NÃO foi entregue, então seta o
				// id_cliente para -1 para indicar que ele foi entregue
				produto->id_cliente = -1;
				atualizar_produtos(arq_produtos, produto); // atualiza o produto no arquivo
				printf("\nproduto \"%s\" entregue com sucesso!\n", produto->nome);
			}
			free(produto); // libera memória
		}
		else
			printf("\nNao existe produto com o ID \"%d\".\n", id_produto);
	}
	else
		printf("\nO ID so pode conter numeros!\n");
 
	// fecha o arquivo
	fclose(arq_produtos);
 
	printf("\nPressione <Enter> para continuar...");
	scanf("%*c");
 
	fseek(stdin, 0, SEEK_END); 
}
void excluir_cliente()
{
	char str_id_cliente[10];
	int id_cliente;
 
	printf("\nDigite o ID do cliente: ");
	scanf("%10s%*c", str_id_cliente);
 
	fseek(stdin, 0, SEEK_END); 
 
	// verifica se str_id_cliente só contém números
	if(str_somente_numeros(str_id_cliente) == 1)
	{
		// se chegou aqui é porque o ID do cliente é válido
		sscanf(str_id_cliente, "%d", &id_cliente);
		// rb abre para leitura (o arquivo deve existir)
		FILE *arq_clientes = fopen("clientes.bin", "rb");
 
		if(arq_clientes == NULL)
		{
			printf("\nFalha ao abrir arquivo(s)!\n");
			exit(1); // aborta o programa
		}
 
		// verifica se o cliente existe
		if(existe_cliente(arq_clientes, id_cliente) == 1)
		{
			char nome_cliente[MAX];
			// abre um novo arquivo temporário
			FILE *arq_temp = fopen("temp_clientes.bin", "a+b");
			if(arq_temp == NULL)
			{
				printf("\nFalha ao criar arquivo temporario!\n");
				fclose(arq_clientes);
				exit(1); // aborta o programa
			}
			rewind(arq_clientes); // vai para o início do arquivo
 
			t_cliente cliente;
			while(1) // loop para percorrer o arquivo
			{
 
				// fread retorna o número de elementos lidos com sucesso
				size_t result = fread(&cliente, sizeof(t_cliente), 1, arq_clientes);
 
				// se for 0, é porque não há mais elemento, então sai do loop
				if(result == 0)
					break;
 
				// só copia pro novo arquivo se for diferente
				if(cliente.id != id_cliente)
				{
					// escreve no arquivo temporário
					fwrite(&cliente, sizeof(t_cliente), 1, arq_temp);
				}
				else
					strcpy(nome_cliente, cliente.nome);
			}
			// antes de fazer operações de remover arquivo e renomear,
			// é preciso fechar os dois arquivos
			fclose(arq_clientes);
			fclose(arq_temp);
 
			// depois de fechar o arquivo, então tentamos remover
			if(remove("clientes.bin") != 0)
				printf("\nErro ao deletar o arquivo \"clientes.bin\"\n");
			else
			{
				// renomeia o arquivo
				int r = rename("temp_clientes.bin", "clientes.bin");
				if(r != 0)
				{
					printf("\nPermissao negada para renomear o arquivo!\n");
					printf("Feche esse programa bem como o arquivo \"temp_clientes.bin\" e renomeie manualmente para \"clientes.bin\"\n");
				}
				else
					printf("\ncliente \"%s\" removido com sucesso!\n", nome_cliente);
			}
		}
		else
		{
			fclose(arq_clientes);
			printf("\nNao existe cliente com o ID \"%d\".\n", id_cliente);
		}
	}
	else
		printf("\nO ID so pode conter numeros!\n");
 
	printf("\nPressione <Enter> para continuar...");
	scanf("%*c");
	fseek(stdin, 0, SEEK_END); 
}
void bonus(){ 
		int percent;
		system(limpar_tela);
		printf("Digite a percentagem de bonificacao desejada: ");
		scanf("%d", &percent);
		system(limpar_tela);
		printf("\n================== CASHBACK RETORNADO  ==================\n\n");
    FILE *arquivo;
    char cc;
    arquivo = fopen("Cashback.dat","r");
    if(arquivo){
        while((cc=getc(arquivo))!= EOF){
            printf("%c",cc);
        }
        system("pause");
        }else{
            printf("Nao foi possivel encontrar nada no arquivo\n");
        return 0;
    } 
    printf("\nPressione enter para voltar ao menu principal\n");
    getchar();
    system("cls");
} 