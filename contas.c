#include <stdio.h>
#include <string.h>

#define ARQ "contas.bin"

typedef struct {
    int    conta;
    char   nome[50];
    double saldo;
    int    ativo;
} Cliente;

FILE *abre(const char *modo) {
    FILE *fp = fopen(ARQ, modo);
    if (!fp) fp = fopen(ARQ, "w+b");
    return fp;
}

long total(FILE *fp) {
    fseek(fp, 0, SEEK_END);
    return ftell(fp) / sizeof(Cliente);
}

void ir(FILE *fp, long pos) {
    fseek(fp, pos * sizeof(Cliente), SEEK_SET);
}

void cadastrar() {
    FILE *fp = abre("r+b");
    Cliente c; long pos;
    printf("Posicao: "); scanf("%ld", &pos);
    printf("Conta: ");   scanf("%d",  &c.conta);
    printf("Nome: ");    scanf(" %49[^\n]", c.nome);
    printf("Saldo: ");   scanf("%lf", &c.saldo);
    c.ativo = 1;
    ir(fp, pos); fwrite(&c, sizeof(Cliente), 1, fp);
    fclose(fp);
    printf("Cadastrado!\n");
}

void consultar() {
    FILE *fp = abre("rb"); Cliente c; int num; long i, t = total(fp);
    printf("Conta: "); scanf("%d", &num);
    for (i = 0; i < t; i++) {
        ir(fp, i); fread(&c, sizeof(Cliente), 1, fp);
        if (c.ativo && c.conta == num) {
            printf("[%ld] Conta:%d Nome:%s Saldo:%.2f\n", i, c.conta, c.nome, c.saldo);
            fclose(fp); return;
        }
    }
    printf("Nao encontrado.\n"); fclose(fp);
}

void atualizar() {
    FILE *fp = abre("r+b"); Cliente c; int num; long i, t = total(fp);
    printf("Conta: "); scanf("%d", &num);
    for (i = 0; i < t; i++) {
        ir(fp, i); fread(&c, sizeof(Cliente), 1, fp);
        if (c.ativo && c.conta == num) {
            printf("Novo saldo: "); scanf("%lf", &c.saldo);
            ir(fp, i); fwrite(&c, sizeof(Cliente), 1, fp);
            printf("Atualizado!\n"); fclose(fp); return;
        }
    }
    printf("Nao encontrado.\n"); fclose(fp);
}

void encerrar() {
    FILE *fp = abre("r+b"); Cliente c; int num; long i, t = total(fp);
    printf("Conta: "); scanf("%d", &num);
    for (i = 0; i < t; i++) {
        ir(fp, i); fread(&c, sizeof(Cliente), 1, fp);
        if (c.ativo && c.conta == num) {
            c.ativo = 0;
            ir(fp, i); fwrite(&c, sizeof(Cliente), 1, fp);
            printf("Conta encerrada.\n"); fclose(fp); return;
        }
    }
    printf("Nao encontrado.\n"); fclose(fp);
}

void listar(FILE *fp) {
    Cliente c; long i, t = total(fp); int algum = 0;
    printf("%-4s %-8s %-20s %10s\n", "Pos", "Conta", "Nome", "Saldo");
    for (i = 0; i < t; i++) {
        ir(fp, i); fread(&c, sizeof(Cliente), 1, fp);
        if (c.ativo) { printf("%-4ld %-8d %-20s %10.2f\n", i, c.conta, c.nome, c.saldo); algum = 1; }
    }
    if (!algum) printf("Sem clientes.\n");
}

int main() {
    int op;
    do {
        printf("\n1.Cadastrar 2.Consultar 3.Atualizar 4.Encerrar 5.Listar 6.Rewind+Listar 7.Sair\nOpcao: ");
        scanf("%d", &op);
        if      (op == 1) cadastrar();
        else if (op == 2) consultar();
        else if (op == 3) atualizar();
        else if (op == 4) encerrar();
        else if (op == 5) { FILE *fp = abre("rb"); listar(fp); fclose(fp); }
        else if (op == 6) { FILE *fp = abre("rb"); rewind(fp); listar(fp); fclose(fp); }
    } while (op != 7);
    return 0;
}
