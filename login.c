// Nome do programa: Sistema de login e cadastro em c
// Objetivo do programa:
// - Cadastro / login em um sistema que guarda os registros de entrada em um arquivo bin�rio.
// - Com um c�digo fonte coerente
// - Sujeito a mudan�as
// Nome do programador: Herton Silveira
// Data de cria��o: 26 / outubro / 2022

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define TAMNOME 60
#define TAMSENHA 30
#define MIN 6 // m�nino de caracters em uma senha ou nome

typedef struct
{
    char Nome[TAMNOME];
    char Senha[TAMSENHA];

} dados;

void cadastro();
void login();
int verifica_Login(char senha[], char nome[]);

void main()
{
    setlocale(LC_ALL, "Portuguese");

    int escolha;

    printf("    Sistema login   \n");
    printf("\nEscolha uma op��o:\n");
    printf("[1] Cadastrar-se\n");
    printf("[2] Fazer login\n");
    printf("[3] Sair\n");

    scanf("%i", &escolha);

    switch (escolha)
    {
    case 1:
        cadastro();
        break;
    case 2:
        login();
        break;
    case 3:
        printf("Saindo...");
        break;

    default:
        printf("Escolha inv�lida.");
        break;
    }
    exit(1);
}

void cadastro()
{
    dados usuario_cadastro;

    FILE *Cadastros;

    Cadastros = fopen("cadastros.dat", "ab"); // cria um arquivo bin�rio para grava��o ou apenas altera um existente

    if (Cadastros == NULL) // Caso n�o consiga abrir o arquivo
    {
        printf("falha ao ler o arquivo");
        exit(1);
    }
    else
    {

        fflush(stdin);
        printf("    Cadastrar novo usuario  \n");
        printf("Nome:");

        fgets(usuario_cadastro.Nome, TAMNOME, stdin);
        usuario_cadastro.Nome[strcspn(usuario_cadastro.Nome, "\n")] = '\0'; // busca \n que o fgets l� e o substitui pelo pelo caracter nulo

        if (verifica_Login("**", usuario_cadastro.Nome) == 10)
        {
            fclose(Cadastros);
            printf("O nome %s j� existe.", usuario_cadastro.Nome); // verifica se o nome de entrada j� existe usando a funcao login
            exit(1);                                               // s� que com uma senha que claramente n�o est� certa,  no caso "**".
        }

        printf("Senha:");
        fgets(usuario_cadastro.Senha, TAMSENHA, stdin);
        usuario_cadastro.Senha[strcspn(usuario_cadastro.Senha, "\n")] = '\0';

        if (strlen(usuario_cadastro.Nome) < MIN || strlen(usuario_cadastro.Senha) < MIN)
        {
            fclose(Cadastros);
            printf("\nSenha e/ou nome invalido(a)s!\n(Senhas e nomes devem ter mais de %i caracteres).", MIN);
            exit(1);
        }

        fwrite(&usuario_cadastro, sizeof(dados), 1, Cadastros); // caso passe pelos tratamentos de string,
        printf("\nSalvo com sucesso!");                         // grava a estrutura dados no arquivo bin�rio cadastros.dat
        fclose(Cadastros);

        exit(1);
    }
}

void login()
{
    dados usuario_login;

    FILE *Login;

    Login = fopen("cadastros.dat", "rb"); // cria um arquivo bin�rio para grava��o ou apenas altera um existente

    if (Login == NULL) // Caso n�o consiga abrir o arquivo
    {
        printf("falha ao ler o arquivo, pois n�o existem cadastros.");
        exit(1);
    }
    else
    {
        fflush(stdin);

        printf("    Login  \n");
        printf("Nome:");
        fgets(usuario_login.Nome, TAMNOME, stdin);
        usuario_login.Nome[strcspn(usuario_login.Nome, "\n")] = '\0';
        printf("Senha:"); // parte de leitura de dados
        fgets(usuario_login.Senha, TAMSENHA, stdin);
        usuario_login.Senha[strcspn(usuario_login.Senha, "\n")] = '\0';

        if (verifica_Login(usuario_login.Senha, usuario_login.Nome) == 0)
        {
            fclose(Login);
            printf("Logado com sucesso.");
            printf("\nBem vindo(a), %s!", usuario_login.Nome);
            exit(1);
        }
        if (verifica_Login(usuario_login.Senha, usuario_login.Nome) == 10)
        { // verificando uam das 3 possibilidades da funcao verifica_Login
            fclose(Login);
            printf("Senha incorreta para %s", usuario_login.Nome);
            exit(1);
        }
        if (verifica_Login(usuario_login.Senha, usuario_login.Nome) == 1)
        {
            fclose(Login);
            printf("Usu�rio %s inexistente", usuario_login.Nome);
            exit(1);
        }
        else
            printf("Algo deu errado... reinicie e tente novamente.");
        exit(1);
    }
}

int verifica_Login(char senha[], char nome[])
{

    // fun��o verifica se usuario est� no documento e senha est� correta 
    // retorno 0 = usuario e senha corretos
    // retorno 10 = apenas usario est� correto e senha incorreta
    // retorno 1 = usuario inexistente

    dados _usuario2;

    FILE *Verifica2;
    Verifica2 = fopen("cadastros.dat", "rb");

    if (Verifica2 == NULL)
    {
        printf("falha ao ler o arquivo");
        exit(1);
    }
    else
    {
        while (fread(&_usuario2, sizeof(dados), 1, Verifica2)) // l� e retorna conforme o tamanho da estrutura
        {
            if (strcmp(_usuario2.Nome, nome) == 0 && strcmp(_usuario2.Senha, senha) == 0) // verifica enquanto houver dados se o nome e a senha est�o corretos
            {
                fclose(Verifica2);
                return 0;
            }
            if (strcmp(_usuario2.Nome, nome) == 0 && strcmp(_usuario2.Senha, senha) != 0)
            {
                fclose(Verifica2);
                return 10;
            }
        }

        fclose(Verifica2);
        return 1;
    }
}
