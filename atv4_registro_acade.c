#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

  
struct exercicio {
    char nome[50];                  
    char musculoAlvo[30];     // estrututura do exrv 
    int series;                     
    int repeticoes;                 
};

//  funcoes declardas
int tamanho(FILE*);                // retorna o n de exercicio cadastrados
void cadastrar(FILE*);             
void consultar(FILE*);             
void gerarRelatorio(FILE*);        
void excluir(FILE*);               

int main() {
    int op;            //variavel que armazena a opcao escolhida
    FILE* arq;                       

   
    if ((arq = fopen("exercicios.dat", "rb+")) == NULL) { 
        
        if ((arq = fopen("exercicios.dat", "wb+")) == NULL) {      // abre o arq e se nao exitir cria um
            printf("Falha ao abrir o arquivo!\n");   
            system("pause");
            return 1;
        }
    }

    do {
        system("CLS");  //limpa a tela 
        printf("\n===== GERENCIADOR DE EXERCICIOS =====\n");
        printf("1. Cadastrar novo exercicio\n");
        printf("2. Consultar exercicio\n");
        printf("3. Gerar relatorio\n");
        printf("4. Excluir exercicio\n");
        printf("5. Sair\n");
        printf("======== Exercicios cadastrados: %d ========\n", tamanho(arq));  
        printf("Opcao: ");
        scanf("%d", &op);           // le qual foi a opcao do usuario

        switch (op) {      //executa as funcoes
            case 1:
                cadastrar(arq);  
                break;
            case 2:
                consultar(arq);  
                break;
            case 3:
                gerarRelatorio(arq);  
                break;
            case 4:
                excluir(arq);  
                break;
            case 5:
                fclose(arq);          // fecha o arq  antes de encerrar o programa
                printf("Programa encerrado.\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");  
                system("pause");
        }
    } while (op != 5);         // repete ate o usuario sair 

    return 0;
}

// funcao que calcula o n de exercicios no arq
int tamanho(FILE* arq) {
    fseek(arq, 0, SEEK_END);  
    return ftell(arq) / sizeof(struct exercicio);  
}

  // cadastro nv
void cadastrar(FILE* arq) {
    struct exercicio novoExercicio;      // struct para armazenar um nv exercicio
    char confirma;  

    printf("\n=== Cadastrar Novo Exercicio ===\n");
    printf("Numero do exercicio: %d\n", tamanho(arq) + 1);  
    while ((getchar()) != '\n' && getchar() != EOF);           // limpa o buffer 

    printf("Nome do exercicio: ");
    fgets(novoExercicio.nome, sizeof(novoExercicio.nome), stdin);
    strtok(novoExercicio.nome, "\n");  

    printf("Musculo alvo: ");
    fgets(novoExercicio.musculoAlvo, sizeof(novoExercicio.musculoAlvo), stdin);
    strtok(novoExercicio.musculoAlvo, "\n");  

    printf("Numero de series: ");
    scanf("%d", &novoExercicio.series);

    printf("Numero de repeticoes: ");
    scanf("%d", &novoExercicio.repeticoes);

    printf("\nConfirma cadastro? <s/n>: ");
    scanf(" %c", &confirma);  

    if (toupper(confirma) == 'S') {  
        printf("\nGravando...\n");
        fseek(arq, 0, SEEK_END);                // move o ponteiro para o fimal do arq
        fwrite(&novoExercicio, sizeof(struct exercicio), 1, arq);        // inseri o ex no arq
        printf("Exercicio cadastrado com sucesso!\n");
    } else {
        printf("Cadastro cancelado.\n");  
    }
    system("pause");
}

   // funcao p/ consultar ex
void consultar(FILE* arq) {
    struct exercicio e;  
    int nr;  

    printf("\n=== Consultar Exercicio ===\n");
    printf("Informe o numero do exercicio: ");
    scanf("%d", &nr);  // le o n do ex

    // verifica se e valido o n do ex
    if (nr > 0 && nr <= tamanho(arq)) {
        fseek(arq, (nr - 1) * sizeof(struct exercicio), SEEK_SET);  
        fread(&e, sizeof(struct exercicio), 1, arq);  //le no arq

        printf("\nExercicio: %s\n", e.nome);
        printf("Musculo alvo: %s\n", e.musculoAlvo);
        printf("Series: %d\n", e.series);
        printf("Repeticoes: %d\n", e.repeticoes);
    } else {
        printf("\nNumero de exercicio invalido!\n");  
    }
    system("pause");
}

  // funcao de gerar relatorio 
void gerarRelatorio(FILE* arq) {
    char nomearq[20];  
    printf("Nome do arquivo de relatorio: ");
    scanf("%s", nomearq);  
    strcat(nomearq, ".txt"); // adc o .txt no arq

    FILE* arqtxt = fopen(nomearq, "w");  
    if (!arqtxt) {  // verifica se tem erro na criacao
        printf("Erro ao criar o arquivo!\n");
        system("pause");
        return;
    }

    
    fprintf(arqtxt, "Exercicio               Musculo Alvo         Series  Repeticoes\n"); // cabecalho do relatorio
    fprintf(arqtxt, "================================================================\n");

    struct exercicio e;
    for (int i = 0; i < tamanho(arq); i++) {  
        fseek(arq, i * sizeof(struct exercicio), SEEK_SET); 
        fread(&e, sizeof(struct exercicio), 1, arq);  
        fprintf(arqtxt, "%-25s %-20s %-6d %-10d\n", e.nome, e.musculoAlvo, e.series, e.repeticoes); // escreve os dados do ex no arq
    }

    fprintf(arqtxt, "================================================================\n");  
    fclose(arqtxt);  
    printf("Relatorio gerado com sucesso!\n");
    system("pause");
}

     // funcao de excluir
void excluir(FILE* arq) {
    struct exercicio e;  
    char confirma; 
    int nr;  

    printf("\n=== Excluir Exercicio ===\n");
    printf("Informe o numero do exercicio: ");
    scanf("%d", &nr);  
    
     if (nr > 0 && nr <= tamanho(arq)) {
        fseek(arq, (nr - 1) * sizeof(struct exercicio), SEEK_SET); 
        fread(&e, sizeof(struct exercicio), 1, arq);
		
		//exibe os dados do ex
        printf("\nExercicio: %s\n", e.nome);
        printf("Musculo alvo: %s\n", e.musculoAlvo);
        printf("Series: %d\n", e.series);
        printf("Repeticoes: %d\n", e.repeticoes);

        printf("\nConfirma exclusao? <s/n>: ");
        while ((getchar()) != '\n' && getchar() != EOF);  // limpa o buffer                 
        scanf(" %c", &confirma);

        if (toupper(confirma) == 'S') {
            strcpy(e.nome, "Excluido");                         
            fseek(arq, (nr - 1) * sizeof(struct exercicio), SEEK_SET);
            fwrite(&e, sizeof(struct exercicio), 1, arq);
            printf("\nExercicio excluido com sucesso!\n");
        } else {
            printf("\nExclusao cancelada.\n");
        }
    } else {
        printf("\nNumero de exercicio invalido!\n");
    }
    system("pause");
}





