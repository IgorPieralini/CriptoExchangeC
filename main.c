// exchange_criptomoedas.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//constantes para limitar, pois o replit tem limitações de memória
#define MAX_USUARIOS 10
#define MAX_TRANSACOES 100

//Estruturas de dados das moedas
typedef struct {
    char tipo[10];
    char moeda[10];
    float valor;
    float taxa;
    char data[20];
} Transacao;

//estas sao as moedas
typedef struct {
    float reais, bitcoin, ethereum, ripple;
} Carteira;

// Estrutura do Usuario
typedef struct {
    char cpf[15];
    char senha[20];
    Carteira carteira;
    int transacoes_count;
    Transacao transacoes[MAX_TRANSACOES];
} Usuario;

// Variáveis globais para desenvolvimento
Usuario usuarios[MAX_USUARIOS];
int total_usuarios = 0;

// Define o valor inicial das cotações
float cotacoes[3] = {300000.0, 15000.0, 7.0}; // Bitcoin, 

// Aqui temos várias funções que serão usadas no programa

//data atual
char* data_atual() {
    static char buffer[20];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    return buffer;
}

// tem por objetivo salvar os usuarios
void salvar_usuarios() {
    FILE* f = fopen("usuarios.dat", "wb");
    if (f) {
        fwrite(&total_usuarios, sizeof(int), 1, f);
        fwrite(usuarios, sizeof(Usuario), total_usuarios, f);
        fclose(f);
    }
}

//carregar os usuarios que foram salvos
void carregar_usuarios() {
    FILE* f = fopen("usuarios.dat", "rb");
    if (f) {
        fread(&total_usuarios, sizeof(int), 1, f);
        fread(usuarios, sizeof(Usuario), total_usuarios, f);
        fclose(f);
    }
}

//Atualiza as cotacoes das moedas
void atualizar_cotacoes() {
    for (int i = 0; i < 3; i++) {
        float variacao = ((rand() % 1001) - 500) / 10000.0;
        cotacoes[i] *= (1 + variacao);
    }
    printf("\nCotações atualizadas:\nBitcoin: %.2f\nEthereum: %.2f\nRipple: %.2f\n",
        cotacoes[0], cotacoes[1], cotacoes[2]);
}

//Cria um novo usuario
void criar_usuario() {
    if (total_usuarios >= MAX_USUARIOS) {
        printf("Limite de usuários atingido.\n");
        return;
    }
    Usuario novo;
    printf("---- Cadastro de Novo Usuário ----\n");
    printf("CPF: ");
    scanf("%s", novo.cpf);
    printf("Senha: ");
    scanf("%s", novo.senha);
    novo.carteira.reais = 0;
    novo.carteira.bitcoin = 0;
    novo.carteira.ethereum = 0;
    novo.carteira.ripple = 0;
    novo.transacoes_count = 0;
    usuarios[total_usuarios++] = novo;
    salvar_usuarios();
    printf("Usuário criado com sucesso!\n");
}


// funcao para verficar senha e cpf do usuario
int autenticar(char* cpf, char* senha) {
    for (int i = 0; i < total_usuarios; i++) {
        if (strcmp(usuarios[i].cpf, cpf) == 0 && strcmp(usuarios[i].senha, senha) == 0)
            return i;
    }
    return -1;
}

//tem por objetivo consultar o saldo atual
void consultar_saldo(Usuario* u) {
    printf("\nSaldo atual:\nReais: R$ %.2f\nBitcoin: %.6f\nEthereum: %.6f\nRipple: %.6f\n",
        u->carteira.reais, u->carteira.bitcoin, u->carteira.ethereum, u->carteira.ripple);
}

//faz depositos
void depositar(Usuario* u) {
    float valor;
    printf("Valor do depósito: R$ ");
    scanf("%f", &valor);
    u->carteira.reais += valor;
    printf("Depósito realizado!\n");
}

//este metodo faz o saque
void sacar(Usuario* u) {
    float valor;
    char senha[20];
    printf("Valor do saque: R$ ");
    scanf("%f", &valor);
    printf("Confirme sua senha: ");
    scanf("%s", senha);
    if (strcmp(u->senha, senha) == 0 && valor <= u->carteira.reais) {
        u->carteira.reais -= valor;
        printf("Saque realizado!\n");
    } else {
        printf("Saque negado. Verifique senha ou saldo.\n");
    }
}

//registra as transacoes que foram feitas
void registrar_transacao(Usuario* u, char* tipo, char* moeda, float valor, float taxa) {
    if (u->transacoes_count >= MAX_TRANSACOES) return;
    Transacao* t = &u->transacoes[u->transacoes_count++];
    strcpy(t->tipo, tipo);
    strcpy(t->moeda, moeda);
    t->valor = valor;
    t->taxa = taxa;
    strcpy(t->data, data_atual());
}

// faz a compra de moedas
void comprar(Usuario* u) {
    int opc;
    float valor, taxa, recebido;
    char* moedas[] = {"bitcoin", "ethereum", "ripple"};
    printf("1. Bitcoin\n2. Ethereum\n3. Ripple\nEscolha: ");
    scanf("%d", &opc);
    printf("Valor (R$): ");
    scanf("%f", &valor);
    char senha[20];
    printf("Senha: ");
    scanf("%s", senha);
    if (strcmp(senha, u->senha) != 0 || valor > u->carteira.reais) {
        printf("Erro na compra.\n");
        return;
    }
    switch(opc) {
        case 1: taxa = 0.02; recebido = (valor * (1 - taxa)) / cotacoes[0]; u->carteira.bitcoin += recebido; break;
        case 2: taxa = 0.01; recebido = (valor * (1 - taxa)) / cotacoes[1]; u->carteira.ethereum += recebido; break;
        case 3: taxa = 0.01; recebido = (valor * (1 - taxa)) / cotacoes[2]; u->carteira.ripple += recebido; break;
        default: return;
    }
    u->carteira.reais -= valor;
    registrar_transacao(u, "compra", moedas[opc-1], valor, taxa);
    printf("Compra realizada com sucesso.\n");
}

// tem por opção vender as moedas
void vender(Usuario* u) {
    int opc;
    float valor, taxa, recebido;
    char* moedas[] = {"bitcoin", "ethereum", "ripple"};
    printf("1. Bitcoin\n2. Ethereum\n3. Ripple\nEscolha: ");
    scanf("%d", &opc);
    printf("Valor (R$): ");
    scanf("%f", &valor);
    switch(opc) {
        case 1: taxa = 0.03; recebido = (valor / cotacoes[0]) * (1 + taxa); if (recebido > u->carteira.bitcoin) return; u->carteira.bitcoin -= recebido; break;
        case 2: taxa = 0.02; recebido = (valor / cotacoes[1]) * (1 + taxa); if (recebido > u->carteira.ethereum) return; u->carteira.ethereum -= recebido; break;
        case 3: taxa = 0.01; recebido = (valor / cotacoes[2]) * (1 + taxa); if (recebido > u->carteira.ripple) return; u->carteira.ripple -= recebido; break;
        default: return;
    }
    u->carteira.reais += valor * (1 - taxa);
    registrar_transacao(u, "venda", moedas[opc-1], valor, taxa);
    printf("Venda realizada.\n");
}

// tem por objetivo gerar o extrato do usuario
void extrato(Usuario* u) {
    char nome_arquivo[30];
    sprintf(nome_arquivo, "extrato_%s.txt", u->cpf);
    FILE* f = fopen(nome_arquivo, "w");
    for (int i = 0; i < u->transacoes_count; i++) {
        Transacao* t = &u->transacoes[i];
        fprintf(f, "%s | %s | R$ %.2f | Taxa %.2f | %s\n", t->tipo, t->moeda, t->valor, t->taxa, t->data);
    }
    fclose(f);
    printf("Extrato salvo em %s\n", nome_arquivo);
}

// a função main é a mais importante, juntando todas as funções
int main() {
    srand(time(NULL));
    carregar_usuarios();

    if (total_usuarios == 0) {
        printf("Nenhum usuário cadastrado.\n");
        criar_usuario(); // cadastrar o primeiro usuário
    }

    char cpf[15], senha[20];
    printf("CPF: "); scanf("%s", cpf);
    printf("Senha: "); scanf("%s", senha);
    int id = autenticar(cpf, senha);

    if (id == -1) {
        printf("Login falhou.\n");
        return 1;
    }

    Usuario* u = &usuarios[id];
    int opc;
    do {
        printf("\n1. Consultar Saldo\n2. Depositar\n3. Sacar\n4. Comprar\n5. Vender\n6. Extrato\n7. Atualizar Cotação\n0. Sair\nOpção: ");
        scanf("%d", &opc);

        // para a opcao selecionada chama a determinada função
        switch(opc) {
            case 1: consultar_saldo(u); break;
            case 2: depositar(u); break;
            case 3: sacar(u); break;
            case 4: comprar(u); break;
            case 5: vender(u); break;
            case 6: extrato(u); break;
            case 7: atualizar_cotacoes(); break;
        }
        salvar_usuarios();
    } while(opc != 0);

    // Enquanto a opção for diferente de 0 o programa continua rodando, assim evitando finalizar

    return 0;
}