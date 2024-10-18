#include "codigo_fonte.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Variável global para gerar códigos de disciplina
static int contador_disciplina = 1;

// Funções auxiliares
void converter_nome(char *nome) {
    for (int i = 0; nome[i]; i++) {
        nome[i] = toupper(nome[i]);
    }
}

// Funções relacionadas a Cursos
void gerar_codigo_disciplina(int *codigo) {
    *codigo = contador_disciplina++;
}

CursoNo* buscar_curso_por_codigo(CursoNo *curso, int codigo) {
    if (curso == NULL || curso->codigo == codigo) {
        return curso;
    }
    if (codigo < curso->codigo) {
        return buscar_curso_por_codigo(curso->esquerda, codigo);
    }
    return buscar_curso_por_codigo(curso->direita, codigo);
}

void adicionar_curso(CursoNo **curso, int codigo, const char *nome, int num_periodos) {
    if (*curso == NULL) {
        CursoNo *novo = (CursoNo *)malloc(sizeof(CursoNo));
        novo->codigo = codigo;
        strcpy(novo->nome, nome);
        novo->num_periodos = num_periodos;
        novo->esquerda = novo->direita = NULL;
        novo->disciplinas = NULL;
        novo->alunos = NULL;
        *curso = novo;
    } else if (codigo < (*curso)->codigo) {
        adicionar_curso(&(*curso)->esquerda, codigo, nome, num_periodos);
    } else if (codigo > (*curso)->codigo) {
        adicionar_curso(&(*curso)->direita, codigo, nome, num_periodos);
    }
}

void mostrar_curso(CursoNo *curso) {
    if (curso != NULL) {
        mostrar_curso(curso->esquerda);
        printf("Código: %d, Nome: %s, Períodos: %d\n", curso->codigo, curso->nome, curso->num_periodos);
        mostrar_curso(curso->direita);
    }
}

// Funções relacionadas a Alunos
void adicionar_aluno(AlunoNo **aluno, int matricula, char *nome, int codigo_curso) {
    AlunoNo *novo = (AlunoNo *)malloc(sizeof(AlunoNo));
    novo->matricula = matricula;
    strcpy(novo->nome, nome);
    converter_nome(novo->nome);
    novo->codigo_curso = codigo_curso;
    novo->notas = NULL;
    novo->matriculas = NULL;
    novo->proximo = *aluno;
    *aluno = novo;
}

void listar_alunos_por_curso(AlunoNo *aluno, int codigo_curso) {
    while (aluno != NULL) {
        if (aluno->codigo_curso == codigo_curso) {
            printf("Matrícula: %d, Nome: %s\n", aluno->matricula, aluno->nome);
        }
        aluno = aluno->proximo;
    }
}

AlunoNo* buscar_aluno_no_curso(CursoNo *curso, int codigo_curso, int matricula) {
    CursoNo *curso_encontrado = buscar_curso_por_codigo(curso, codigo_curso);
    if (curso_encontrado == NULL) return NULL;

    AlunoNo *aluno = curso_encontrado->alunos;
    while (aluno != NULL) {
        if (aluno->matricula == matricula) {
            return aluno;
        }
        aluno = aluno->proximo;
    }
    return NULL; // Aluno não encontrado
}

void mostrar_alunos(AlunoNo *alunos) {
    while (alunos != NULL) {
        printf("Matrícula: %d, Nome: %s, Código do Curso: %d\n", alunos->matricula, alunos->nome, alunos->codigo_curso);
        alunos = alunos->proximo;
    }
}

void consultar_historico_aluno(AlunoNo *aluno, CursoNo *curso, int matricula) {
    AlunoNo *aluno_encontrado = buscar_aluno_no_curso(curso, aluno->codigo_curso, matricula);
    if (aluno_encontrado == NULL) return;

    printf("Matrícula: %d, Nome: %s\n", aluno_encontrado->matricula, aluno_encontrado->nome);
    printf("Histórico:\n");
    // Implementar lógica para mostrar histórico
}

// Funções relacionadas a Matrículas
void adicionar_matricula(AlunoNo **aluno, int codigo_disciplina, int matricula) {
    AlunoNo *aluno_encontrado = *aluno;
    while (aluno_encontrado != NULL) {
        if (aluno_encontrado->matricula == matricula) {
            MatriculaNo *matricula_nova = (MatriculaNo *)malloc(sizeof(MatriculaNo));
            matricula_nova->codigo_disciplina = codigo_disciplina;
            matricula_nova->esquerda = matricula_nova->direita = NULL;
            matricula_nova->codigo_disciplina = codigo_disciplina;
            matricula_nova->esquerda = aluno_encontrado->matriculas;
            aluno_encontrado->matriculas = matricula_nova;
            return;
        }
        aluno_encontrado = aluno_encontrado->proximo;
    }
}

void remover_matricula_aluno(MatriculaNo **matricula, int codigo_disciplina) {
    if (*matricula == NULL) return;

    if ((*matricula)->codigo_disciplina == codigo_disciplina) {
        MatriculaNo *temp = *matricula;
        *matricula = (*matricula)->esquerda;
        free(temp);
        return;
    }

    remover_matricula_aluno(&(*matricula)->esquerda, codigo_disciplina);
    remover_matricula_aluno(&(*matricula)->direita, codigo_disciplina);
}

void mostrar_matriculas(MatriculaNo *matriculas) {
    while (matriculas != NULL) {
        printf("Código da Disciplina: %d\n", matriculas->codigo_disciplina);
        matriculas = matriculas->esquerda;
    }
}

int verificar_matriculas(AlunoNo *aluno, int codigo_disciplina) {
    MatriculaNo *matricula = aluno->matriculas;
    while (matricula != NULL) {
        if (matricula->codigo_disciplina == codigo_disciplina) {
            return 1; // Matrícula encontrada
        }
        matricula = matricula->esquerda;
    }
    return 0; // Matrícula não encontrada
}

// Funções relacionadas a Notas
int adicionar_nota(AlunoNo **aluno, int matricula, int codigo_disciplina, int semestre, float nota_final) {
    AlunoNo *aluno_encontrado = *aluno;
    while (aluno_encontrado != NULL) {
        if (aluno_encontrado->matricula == matricula) {
            NotaNo *nota_nova = (NotaNo *)malloc(sizeof(NotaNo));
            nota_nova->codigo_disciplina = codigo_disciplina;
            nota_nova->semestre = semestre;
            nota_nova->nota_final = nota_final;
            nota_nova->esquerda = nota_nova->direita = NULL;
            nota_nova->codigo_disciplina = codigo_disciplina;
            nota_nova->esquerda = aluno_encontrado->notas;
            aluno_encontrado->notas = nota_nova;
            return 1; // Nota adicionada com sucesso
        }
        aluno_encontrado = aluno_encontrado->proximo;
    }
    return 0; // Aluno não encontrado
}

void exibir_notas_aluno_disciplina(AlunoNo *aluno, CursoNo *curso, int matricula, int codigo_disciplina) {
    AlunoNo *aluno_encontrado = buscar_aluno_no_curso(curso, aluno->codigo_curso, matricula);
    if (aluno_encontrado == NULL) return;

    NotaNo *nota = aluno_encontrado->notas;
    while (nota != NULL) {
        if (nota->codigo_disc iplina == codigo_disciplina) {
            printf("Nota Final: %.2f, Semestre: %d\n", nota->nota_final, nota->semestre);
            return;
        }
        nota = nota->esquerda;
    }
}

void listar_notas_disciplina_por_periodo(AlunoNo *aluno, int periodo, int matricula) {
    AlunoNo *aluno_encontrado = buscar_aluno_no_curso(aluno, aluno->codigo_curso, matricula);
    if (aluno_encontrado == NULL) return;

    NotaNo *nota = aluno_encontrado->notas;
    while (nota != NULL) {
        if (nota->semestre == periodo) {
            printf("Código da Disciplina: %d, Nota Final: %.2f\n", nota->codigo_disciplina, nota->nota_final);
        }
        nota = nota->esquerda;
    }
}

int listar_notas_por_periodo(NotaNo *nota, int periodo) {
    if (nota == NULL) return 0;

    if (nota->semestre == periodo) {
        printf("Código da Disciplina: %d, Nota Final: %.2f\n", nota->codigo_disciplina, nota->nota_final);
        return 1;
    }

    return listar_notas_por_periodo(nota->esquerda, periodo) || listar_notas_por_periodo(nota->direita, periodo);
}

void mostrar_notas_aluno(AlunoNo *aluno, DisciplinaNo *disciplinas, int periodo) {
    NotaNo *nota = aluno->notas;
    while (nota != NULL) {
        if (nota->semestre == periodo) {
            printf("Código da Disciplina: %d, Nota Final: %.2f\n", nota->codigo_disciplina, nota->nota_final);
        }
        nota = nota->esquerda;
    }
}

// Funções relacionadas a Disciplinas
void mostrar_disciplinas_por_periodo(CursoNo *curso, int periodo) {
    DisciplinaNo *disciplina = curso->disciplinas;
    while (disciplina != NULL) {
        if (disciplina->periodo == periodo) {
            printf("Código da Disciplina: %d, Nome: %s, Carga Horária: %d\n", disciplina->codigo, disciplina->nome, disciplina->carga_horaria);
        }
        disciplina = disciplina->esquerda;
    }
}

int adicionar_disciplina(CursoNo **curso, DisciplinaNo *disciplina, int codigo_curso) {
    CursoNo *curso_encontrado = buscar_curso_por_codigo(*curso, codigo_curso);
    if (curso_encontrado == NULL) return 0;

    DisciplinaNo *disciplina_nova = (DisciplinaNo *)malloc(sizeof(DisciplinaNo));
    disciplina_nova->codigo = disciplina->codigo;
    strcpy(disciplina_nova->nome, disciplina->nome);
    disciplina_nova->periodo = disciplina->periodo;
    disciplina_nova->carga_horaria = disciplina->carga_horaria;
    disciplina_nova->esquerda = disciplina_nova->direita = NULL;
    disciplina_nova->codigo = disciplina->codigo;
    disciplina_nova->esquerda = curso_encontrado->disciplinas;
    curso_encontrado->disciplinas = disciplina_nova;
    return 1; // Disciplina adicionada com sucesso
}

void listar_disciplinas_por_curso(CursoNo *curso, int codigo_curso) {
    CursoNo *curso_encontrado = buscar_curso_por_codigo(curso, codigo_curso);
    if (curso_encontrado == NULL) return;

    DisciplinaNo *disciplina = curso_encontrado->disciplinas;
    while (disciplina != NULL) {
        printf("Código da Disciplina: %d, Nome: %s, Período: %d, Carga Horária: %d\n", disciplina->codigo, disciplina->nome, disciplina->periodo, disciplina->carga_horaria);
        disciplina = disciplina->esquerda;
    }
}

int remover_disciplina_do_curso(CursoNo **curso, AlunoNo *alunos, int codigo_curso, int codigo_disciplina) {
    CursoNo *curso_encontrado = buscar_curso_por_codigo(*curso, codigo_curso);
    if (curso_encontrado == NULL) return 0;

    DisciplinaNo *disciplina_encontrada = NULL;
    DisciplinaNo *disciplina_anterior = NULL;
    DisciplinaNo *disciplina_atual = curso_encontrado->disciplinas;

    while (disciplina_atual != NULL) {
        if (disciplina_atual->codigo == codigo_disciplina) {
            disciplina_encontrada = disciplina_atual;
            break;
        }
        disciplina_anterior = disciplina_atual;
        disciplina_atual = disciplina_atual->esquerda;
    }

    if (disciplina_encontrada == NULL) return 0;

    if (disciplina_anterior == NULL) {
        curso_encontrado->disciplinas = disciplina_encontrada->esquerda;
    } else {
        disciplina_anterior->esquerda = disciplina_encontrada->esquerda;
    }

    free(disciplina_encontrada);
    return 1; // Disciplina removida com sucesso
}

void listar_disciplinas_por_aluno(AlunoNo *aluno, CursoNo *curso, int matricula) {
    AlunoNo *aluno_encontrado = buscar_aluno_no_curso(curso, aluno->codigo_curso, matricula);
    if (aluno_encontrado == NULL) return;

    MatriculaNo *matricula_atual = aluno_encontrado->matriculas;
    while (matricula_atual != NULL) {
        printf("Código da Disciplina: %d\n", matricula_atual->codigo_disciplina);
        matricula_atual = matricula_atual->esquerda;
    }
}
