#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codigo_fonte/codigo_fonte.h"

void menu_geral() {
    printf(
        "======================= MENU =======================\n"
        "[ 1 ] Cadastrar Aluno\n"
        "[ 2 ] Cadastrar Curso\n"
        "[ 3 ] Cadastrar Disciplina\n"
        "[ 4 ] Cadastrar Matrícula\n"
        "[ 5 ] Cadastrar Nota\n"
        "[ 6 ] Mostrar Alunos por Curso\n"
        "[ 7 ] Mostrar Cursos Disponíveis\n"
        "[ 8 ] Mostrar Disciplinas por Curso\n"
        "[ 9 ] Mostrar Disciplinas por Período\n"
        "[10 ] Mostrar Disciplinas Matriculadas\n"
        "[11 ] Mostrar Notas por Período\n"
        "[12 ] Consultar Nota de Disciplina\n"
        "[13 ] Remover Disciplina de Curso\n"
        "[14 ] Remover Disciplina da Matrícula\n"
        "[15 ] Consultar Histórico do Aluno\n"
        "[16 ] Sair\n"
        "====================================================\n"
        "Selecione uma opção: ");
}

int main() {
    int opcao_menu;

    AlunoNo *alunos = NULL;
    CursoNo *arvore_curso = NULL;
    NotaNo *arvore_notas = NULL;
    DisciplinaNo *arvore_disciplina = NULL;
    MatriculaNo *arvore_matricula = NULL;

    int matricula, codigo_curso, quantidade_periodos, codigo_disciplina, matricula_disciplina, codigo_gerado;
    float periodo;
    char nome[100], nome_curso[100];

    do {
        menu_geral();
        scanf("%d", &opcao_menu);

        switch (opcao_menu) {
            case 1:
                // Cadastrar Aluno
                if (arvore_curso == NULL) {
                    printf("Erro: Nenhum curso cadastrado. Cadastre um curso primeiro.\n");
                    break;
                }
                printf("Informe o nome do aluno: ");
                scanf(" %[^\n]s", nome); 
                printf("Informe a matrícula: ");
                scanf("%d", &matricula);
                printf("Informe o código do curso: ");
                scanf("%d", &codigo_curso);
                adicionar_aluno(&alunos, matricula, nome, codigo_curso);
                break;

            case 2:
                // Cadastrar Curso
                printf("Informe o código do curso: ");
                scanf("%d", &codigo_curso);
                printf("Informe o nome do curso: ");
                scanf(" %[^\n]s", nome_curso);
                printf("Informe a quantidade de períodos: ");
                scanf("%d", &quantidade_periodos);
                adicionar_curso(&arvore_curso, codigo_curso, nome_curso, quantidade_periodos);
                break;

            case 3:
                // Cadastrar Disciplina
                if (arvore_curso == NULL) {
                    printf("Erro: Nenhum curso cadastrado. Cadastre um curso primeiro.\n");
                    break;
                }
                DisciplinaNo *disciplina = (DisciplinaNo *)malloc(sizeof(DisciplinaNo));
                if (!disciplina) {
                    printf("Erro ao alocar memória para disciplina.\n");
                    break;
                }
                printf("Digite o nome da disciplina: ");
                scanf(" %[^\n]s", disciplina->nome);
                printf("Digite o código do curso: ");
                scanf("%d", &codigo_curso);
                printf("Digite a carga horária da disciplina: ");
                scanf("%d", &disciplina->carga_horaria);
                printf("Digite o período da disciplina: ");
                scanf("%d", &disciplina->periodo);

                CursoNo *curso_encontrado = buscar_curso_por_codigo(arvore_curso, codigo_curso);
                if (!curso_encontrado) {
                    printf("Erro: Curso não encontrado.\n");
                    free(disciplina);
                    break;
                }

                gerar_codigo_disciplina(&codigo_gerado);
                disciplina->codigo = codigo_gerado;
                int resultado = adicionar_disciplina(&curso_encontrado, disciplina, codigo_curso);
                if (resultado) {
                    printf("Disciplina cadastrada com sucesso! Código gerado: %d\n", codigo_gerado);
                } else {
                    printf("Erro ao cadastrar disciplina.\n");
                    free(disciplina);
                }
                break;

            case 16:
                // Sair
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
    } while (opcao_menu != 16);

    return 0;
}
