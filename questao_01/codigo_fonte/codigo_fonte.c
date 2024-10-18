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
    if (aluno_encontrado == NULL) {
        printf("Aluno não encontrado.\n");
        return;
    }

    MatriculaNo *matricula_atual = aluno_encontrado->matriculas;
    while (matricula_atual != NULL) {
        DisciplinaNo *disciplina = buscar_disciplina(curso->disciplinas, matricula_atual->codigo_disciplina);
        if (disciplina != NULL) {
            printf("Código: %d, Nome: %s, Período: %d, Carga Horária: %d\n", 
                   disciplina->codigo, disciplina->nome, disciplina->periodo, disciplina->carga_horaria);
        }
        matricula_atual = matricula_atual->esquerda;
    }
}

// Funções auxiliares adicionais

DisciplinaNo* buscar_disciplina(DisciplinaNo *disciplina, int codigo) {
    if (disciplina == NULL || disciplina->codigo == codigo) {
        return disciplina;
    }
    if (codigo < disciplina->codigo) {
        return buscar_disciplina(disciplina->esquerda, codigo);
    }
    return buscar_disciplina(disciplina->direita, codigo);
}

void remover_folha_matricula(MatriculaNo **raiz, MatriculaNo *pai, MatriculaNo *atual) {
    if (pai == NULL) {
        *raiz = NULL;
    } else if (pai->esquerda == atual) {
        pai->esquerda = NULL;
    } else {
        pai->direita = NULL;
    }
    free(atual);
}

void remover_com_um_filho_matricula(MatriculaNo **raiz, MatriculaNo *pai, MatriculaNo *atual) {
    MatriculaNo *filho = (atual->esquerda != NULL) ? atual->esquerda : atual->direita;

    if (pai == NULL) {
        *raiz = filho;
    } else if (pai->esquerda == atual) {
        pai->esquerda = filho;
    } else {
        pai->direita = filho;
    }
    free(atual);
}

void remover_com_dois_filhos_matricula(MatriculaNo *atual) {
    MatriculaNo *sucessor = atual->direita;
    MatriculaNo *pai_sucessor = atual;

    while (sucessor->esquerda != NULL) {
        pai_sucessor = sucessor;
        sucessor = sucessor->esquerda;
    }

    atual->codigo_disciplina = sucessor->codigo_disciplina;

    if (pai_sucessor == atual) {
        pai_sucessor->direita = sucessor->direita;
    } else {
        pai_sucessor->esquerda = sucessor->direita;
    }

    free(sucessor);
}

AlunoNo* localizar_aluno(AlunoNo *aluno, int matricula) {
    while (aluno != NULL && aluno->matricula != matricula) {
        aluno = aluno->proximo;
    }
    return aluno;
}

void listar_disciplinas_por_aluno(AlunoNo *aluno, CursoNo *curso, int matricula) {
    AlunoNo *aluno_encontrado = buscar_aluno_no_curso(curso, aluno->codigo_curso, matricula);
    if (aluno_encontrado == NULL) {
        printf("Aluno não encontrado.\n");
        return;
    }

    MatriculaNo *matricula_atual = aluno_encontrado->matriculas;
    while (matricula_atual != NULL) {
        DisciplinaNo *disciplina = buscar_disciplina(curso->disciplinas, matricula_atual->codigo_disciplina);
        if (disciplina != NULL) {
            printf("Código: %d, Nome: %s, Período: %d, Carga Horária: %d\n", 
                   disciplina->codigo, disciplina->nome, disciplina->periodo, disciplina->carga_horaria);
        }
        matricula_atual = matricula_atual->esquerda;
    }
}

// Funções auxiliares adicionais

DisciplinaNo* buscar_disciplina(DisciplinaNo *disciplina, int codigo) {
    if (disciplina == NULL || disciplina->codigo == codigo) {
        return disciplina;
    }
    if (codigo < disciplina->codigo) {
        return buscar_disciplina(disciplina->esquerda, codigo);
    }
    return buscar_disciplina(disciplina->direita, codigo);
}

void remover_folha_matricula(MatriculaNo **raiz, MatriculaNo *pai, MatriculaNo *atual) {
    if (pai == NULL) {
        *raiz = NULL;
    } else if (pai->esquerda == atual) {
        pai->esquerda = NULL;
    } else {
        pai->direita = NULL;
    }
    free(atual);
}

void remover_com_um_filho_matricula(MatriculaNo **raiz, MatriculaNo *pai, MatriculaNo *atual) {
    MatriculaNo *filho = (atual->esquerda != NULL) ? atual->esquerda : atual->direita;

    if (pai == NULL) {
        *raiz = filho;
    } else if (pai->esquerda == atual) {
        pai->esquerda = filho;
    } else {
        pai->direita = filho;
    }
    free(atual);
}

void remover_com_dois_filhos_matricula(MatriculaNo *atual) {
    MatriculaNo *sucessor = atual->direita;
    MatriculaNo *pai_sucessor = atual;

    while (sucessor->esquerda != NULL) {
        pai_sucessor = sucessor;
        sucessor = sucessor->esquerda;
    }

    atual->codigo_disciplina = sucessor->codigo_disciplina;

    if (pai_sucessor == atual) {
        pai_sucessor->direita = sucessor->direita;
    } else {
        pai_sucessor->esquerda = sucessor->direita;
    }

    free(sucessor);
}

AlunoNo* localizar_aluno(AlunoNo *aluno, int matricula) {
    while (aluno != NULL && aluno->matricula != matricula) {
        aluno = aluno->proximo;
    }
    return aluno;
}

void localizar_matricula(MatriculaNo *matricula, int codigo_disciplina, int *encontrado) {
    if (matricula == NULL) {
        *encontrado = 0;
        return;
    }

    if (matricula->codigo_disciplina == codigo_disciplina) {
        *encontrado = 1;
        return;
    }

    if (codigo_disciplina < matricula->codigo_ disciplina) {
        localizar_matricula(matricula->esquerda, codigo_disciplina, encontrado);
    } else {
        localizar_matricula(matricula->direita, codigo_disciplina, encontrado);
    }
}
// Funções adicionais

void exibir_disciplinas(DisciplinaNo *disciplina, int codigo_disciplina) {
    if (disciplina != NULL) {
        if (disciplina->codigo == codigo_disciplina) {
            printf("Código: %d\n", disciplina->codigo);
            printf("Nome: %s\n", disciplina->nome);
            printf("Carga horária: %d\n", disciplina->carga_horaria);
            printf("Período: %d\n", disciplina->periodo);
            printf("\n");
        } else if (codigo_disciplina < disciplina->codigo) {
            exibir_disciplinas(disciplina->esquerda, codigo_disciplina);
        } else {
            exibir_disciplinas(disciplina->direita, codigo_disciplina);
        }
    }
}

void exibir_disciplina(CursoNo *curso, int codigo_disciplina, int codigo_curso) {
    if (curso != NULL) {
        if (curso->codigo == codigo_curso) {
            if (curso->disciplinas != NULL) {
                exibir_disciplinas(curso->disciplinas, codigo_disciplina);
            }
        } else if (codigo_curso < curso->codigo) {
            exibir_disciplina(curso->esquerda, codigo_disciplina, codigo_curso);
        } else {
            exibir_disciplina(curso->direita, codigo_disciplina, codigo_curso);
        }
    }
}

void exibir_disciplina_aluno(MatriculaNo *mat, CursoNo *cursos, int codigo_curso) {
    if (mat != NULL) {
        exibir_disciplina(cursos, mat->codigo_disciplina, codigo_curso);
        exibir_disciplina_aluno(mat->esquerda, cursos, codigo_curso);
        exibir_disciplina_aluno(mat->direita, cursos, codigo_curso);
    }
}

void exibir_disciplinasporaluno(AlunoNo *aluno, CursoNo *cursos, int matricula) {
    if (aluno != NULL) {
        if (aluno->matricula == matricula) {
            exibir_disciplina_aluno(aluno->matriculas, cursos, aluno->codigo_curso);
        } else {
            exibir_disciplinasporaluno(aluno->proximo, cursos, matricula);
        }
    }
}

void aux_XI_notaperiodo(NotaNo *nota, int periodo) {
    if (nota != NULL) {
        if (nota->semestre == periodo) {
            printf("Código: %d\n", nota->codigo_disciplina);
            printf("Nota Final: %.2f\n", nota->nota_final);
            printf("Semestre: %.2f\n", nota->semestre);
            printf("\n");
        }
        aux_XI_notaperiodo(nota->esquerda, periodo);
        aux_XI_notaperiodo(nota->direita, periodo);
    }
}

void notas_disciplina_periodo_aluno(AlunoNo *aluno, int periodo, int matricula) {
    if (aluno != NULL) {
        if (aluno->matricula == matricula) {
            aux_XI_notaperiodo(aluno->notas, periodo);
        } else {
            notas_disciplina_periodo_aluno(aluno->proximo, periodo, matricula);
        }
    }
}

void exibir_nota_aluno_disciplina(AlunoNo *aluno, CursoNo *curso, int matricula, int codigo_disciplina) {
    if (aluno != NULL) {
        if (aluno->matricula == matricula) {
            NotaNo *nota = aluno->notas;
            while (nota != NULL) {
                if (nota->codigo_disciplina == codigo_disciplina) {
                    Discipli No *disciplina = curso->disciplinas;
                    while (disciplina != NULL) {
                        if (disciplina->codigo == codigo_disciplina) {
                            printf("Aluno: %s\n", aluno->nome);
                            printf("Disciplina: %d\n", nota->codigo_disciplina);
                            printf("Período: %d\n", disciplina->periodo);
                            printf("Carga Horária: %d\n", disciplina->carga_horaria);
                            printf("Nota Final: %.2f\n", nota->nota_final);
                        }
                        if (codigo_disciplina < disciplina->codigo) {
                            disciplina = disciplina->esquerda;
                        } else {
                            disciplina = disciplina->direita;
                        }
                    }
                }
                if (codigo_disciplina < nota->codigo_disciplina) {
                    nota = nota->esquerda;
                } else {
                    nota = nota->direita;
                }
            }
        } else {
            exibir_nota_aluno_disciplina(aluno->proximo, curso, matricula, codigo_disciplina);
        }
    }
}

int e_folha(DisciplinaNo *disciplina) {
    return (disciplina->esquerda == NULL && disciplina->direita == NULL);
}

DisciplinaNo* so_um_filho(DisciplinaNo *disciplina) {
    DisciplinaNo *aux = NULL;

    if (disciplina->direita == NULL) {
        aux = disciplina->esquerda;
    } else if (disciplina->esquerda == NULL) {
        aux = disciplina->direita;
    }

    return aux;
}

DisciplinaNo* menor_filho_esquerda(DisciplinaNo *disciplina) {
    DisciplinaNo *aux = NULL;

    if (disciplina != NULL) {
        aux = menor_filho_esquerda(disciplina->esquerda);
        if (!aux) {
            aux = disciplina;
        }
    }

    return aux;
}

void remover_disciplina(DisciplinaNo **disciplina, int codigo_disciplina, int *remove) {
    if (*disciplina != NULL) {
        DisciplinaNo *aux;
        DisciplinaNo *endereco_filho;
        DisciplinaNo *endereco_menor_filho;

        if ((*disciplina)->codigo == codigo_disciplina) {
            if (e_folha(*disciplina)) {
                aux = *disciplina;
                free(aux);
                *disciplina = NULL;
            } else if ((endereco_filho = so_um_filho(*disciplina)) != NULL) {
                aux = *disciplina;
                free(aux);
                *disciplina = endereco_filho;
            } else {
                endereco_menor_filho = menor_filho_esquerda((*disciplina)->direita);
                (*disciplina)->codigo = endereco_menor_filho->codigo;
                (*disciplina)->carga_horaria = endereco_menor_filho->carga_horaria;
                (*disciplina)->periodo = endereco_menor_filho->periodo;
                strcpy((*disciplina)->nome, endereco_menor_filho->nome);
                remover_disciplina(&(*disciplina)->direita, endereco_menor_filho->codigo, remove);
            }
            *remove = 1;
        } else if (codigo_disciplina < (*disciplina)->codigo) {
            remover_disciplina(&(*disciplina)->esquerda, codigo_disciplina, remove);
        } else {
            remover_disciplina(&(*disciplina)->direita, codigo_disciplina, remove);
        }
    }
}

void confirmar_remocao_disciplina(AlunoNo *alunos, int codigo_disciplina, int *confirmar) {
    int enc = 0;

    if (alunos != NULL) {
        buscar_matricula(alunos->matriculas, codigo_disciplina, &enc);

        if (enc != 0) {
            *confirmar = 1;
        }

        confirmar_remocao_disciplina(alunos->proximo, codigo_disciplina, confirmar);
    }
}

int remover_disciplina_curso(CursoNo **cursos, AlunoNo *alunos, int idcurso, int codigo_disciplina) {
    int remove = 0, confirmar_disciplina = 0;

    if (*cursos != NULL) {
        confirmar_remocao_disciplina(alunos, codigo_disciplina, &confirmar_disciplina);

        if (confirmar_disciplina != 0) {
            if ((*cursos)->codigo == idcurso) {
                remover_disciplina(&(*cursos)->disciplinas, codigo_disciplina, &remove);
            } else if (idcurso < (*cursos)->codigo) {
                remove = remover_disciplina_curso(&(*cursos)->esquerda, alunos, idcurso, codigo_disciplina);
            } else {
                remove = remover _disciplina_curso(&(*cursos)->direita, alunos, idcurso, codigo_disciplina);
            }
        }
    }

    return remove;
}

int e_folha_matricula(MatriculaNo *matricula) {
    return (matricula->esquerda == NULL && matricula->direita == NULL);
}

MatriculaNo* soumfilhomat(MatriculaNo *matricula) {
    MatriculaNo *aux = NULL;

    if (matricula->esquerda == NULL) {
        aux = matricula->direita;
    } else if (matricula->direita == NULL) {
        aux = matricula->esquerda;
    }

    return aux;
}

MatriculaNo* menorfilhoesqmat(MatriculaNo *matricula) {
    MatriculaNo *aux = NULL;

    if (matricula != NULL) {
        aux = menorfilhoesqmat(matricula->esquerda);
        if (!aux) {
            aux = matricula;
        }
    }

    return aux;
}

void remover_matricula(MatriculaNo **matricula, int codigo) {
    if (*matricula != NULL) {
        MatriculaNo *aux;

        if ((*matricula)->codigo_disciplina == codigo) {
            if (e_folha_matricula(*matricula)) {
                aux = *matricula;
                free(aux);
                *matricula = NULL;
            } else if ((aux = soumfilhomat(*matricula)) != NULL) {
                MatriculaNo *temp;
                temp = *matricula;
                free(temp);
                *matricula = aux;
            } else {
                MatriculaNo *menorfilho = menorfilhoesqmat((*matricula)->direita);
                (*matricula)->codigo_disciplina = menorfilho->codigo_disciplina;
                remover_matricula(&(*matricula)->direita, menorfilho->codigo_disciplina);
            }
        } else if (codigo < (*matricula)->codigo_disciplina) {
            remover_matricula(&(*matricula)->esquerda, codigo);
        } else {
            remover_matricula(&(*matricula)->direita, codigo);
        }
    }
}

void exibir_disciplina_historico(DisciplinaNo *disciplina, int codigo_disciplina) {
    if (disciplina != NULL) {
        if (disciplina->codigo == codigo_disciplina) {
            printf("Disciplina: %s\n", disciplina->nome);
        } else if (codigo_disciplina < disciplina->codigo) {
            exibir_disciplina_historico(disciplina->esquerda, codigo_disciplina);
        } else {
            exibir_disciplina_historico(disciplina->direita, codigo_disciplina);
        }
    }
}

void exibir_notas_alunos(NotaNo *nota, DisciplinaNo *disciplina, int periodo) {
    if (nota != NULL) {
        if (nota->semestre == periodo) {
            exibir_disciplina_historico(disciplina, nota->codigo_disciplina);
            printf("\n------------------\n");
            printf("Semestre: %.1f\n\n", nota->semestre);
            printf("Nota: %.2f\n", nota->nota_final);
            printf("\n------------------\n");
        }
        exibir_notas_alunos(nota->esquerda, disciplina, periodo);
        exibir_notas_alunos(nota->direita, disciplina, periodo);
    }
}

int exibir_nome_do_curso(CursoNo *curso, int codigo_curso) {
    int count_periodos = 0;

    if (curso != NULL) {
        if (curso->codigo == codigo_curso) {
            printf("Curso: %s\n", curso->nome);
            count_periodos = curso->num_periodos;
        } else if (codigo_curso < curso->codigo) {
            count_periodos = exibir_nome_do_curso(curso->esquerda, codigo_curso);
        } else {
            count_periodos = exibir_nome_do_curso(curso->direita, codigo_curso);
        }
    }

    return count_periodos;
}

void consultar_historico(AlunoNo *aluno, CursoNo *curso, int matricula) {
    if (aluno != NULL) {
        if (aluno->matricula == matricula) {
            printf("\n------------------\n");
            printf("Matricula: %d\n", aluno->matricula);
            printf("Aluno: %s\n", aluno->nome);
            printf("\n------------------\n");
            int count_periodos = 0;
            count_periodos = exibir_nome_do_curso(curso, aluno->codigo_curso);
            printf("Historico:\n");
            for (int i = 0; i < count_periodos ; i++) {
                exibir_notas_alunos(aluno->notas, curso->disciplinas, i + 1);
            }
        } else {
            consultar_historico(aluno->proximo, curso, matricula);
        }
    }
}

// Funções auxiliares adicionais

ArvoreCursos* buscar_curso(ArvoreCursos *curso, int codigo_curso) {
    ArvoreCursos *resultado = NULL;

    if (curso != NULL) {
        if (curso->codigo == codigo_curso) {
            resultado = curso;
        } else if (codigo_curso < curso->codigo) {
            resultado = buscar_curso(curso->esquerda, codigo_curso);
        } else {
            resultado = buscar_curso(curso->direita, codigo_curso);
        }
    }

    return resultado;
}

void exibir_matriculas(MatriculaNo *r) {
    if (r != NULL) {
        printf("Matricula: %d\n", r->codigo_disciplina);
        exibir_matriculas(r->esquerda);
        exibir_matriculas(r->direita);
    }
}

void buscar_matricula(MatriculaNo *r, int codigo, int *encontrado) {
    if (r != NULL) {
        if (r->codigo_disciplina == codigo) {
            *encontrado = 1;
        } else {
            if (codigo < r->codigo_disciplina) {
                buscar_matricula(r->esquerda, codigo, encontrado);
            } else {
                buscar_matricula(r->direita, codigo, encontrado);
            }
        }
    }
}

AlunoNo* buscar_aluno_por_matricula_no_curso(CursoNo *curso, int codigo_curso, int matricula) {
    AlunoNo *aluno;
    if (curso != NULL) {
        if (curso->codigo == codigo_curso) {
            aluno = curso->alunos;
            while (aluno != NULL && aluno->matricula != matricula) {
                aluno = aluno->proximo;
            }
        } else {
            aluno = buscar_aluno_por_matricula_no_curso(curso->esquerda, codigo_curso, matricula);
            if (aluno == NULL) {
                aluno = buscar_aluno_por_matricula_no_curso(curso->direita, codigo_curso, matricula);
            }
        }

        return aluno;
    }
}

int notas_disciplina_periodo(NotaNo *n, int periodo) {
    if (n == NULL) {
        printf("Nota nao encontrada\n");
    } else {
        if (n->semestre == periodo) {
            printf("Disciplina: %d, Nota Final: %.2f\n", n->codigo_disciplina, n->nota_final);
        }

        notas_disciplina_periodo(n->esquerda, periodo);
        notas_disciplina_periodo(n->direita, periodo);
    }
}

void localizar_no(MatriculaNo **atual, MatriculaNo **pai, int codigo_disciplina, int *encontrado) {
    while (*atual != NULL && *encontrado == 0) {
        if ((*atual)->codigo_disciplina == codigo_disciplina) {
            *encontrado = 1;
        } else {
            *pai = *atual;

            if (codigo_disciplina < (*atual)->codigo_disciplina) {
                *atual = (*atual)->esquerda;
            } else {
                *atual = (*atual)->direita;
            }
        }
    }
}

void remover_no(MatriculaNo **raiz, MatriculaNo *pai, MatriculaNo *atual) {
    if (atual->esquerda == NULL && atual->direita == NULL) {
        remover_no_folha(raiz, pai, atual);
    } else if (atual->esquerda == NULL || atual->direita == NULL) {
        remover_no_com_um_filho(raiz, pai, atual);
    } else {
        remover_no_com_dois_filhos(atual);
    }

    free(atual);
}

void remover_no_folha(MatriculaNo **raiz, MatriculaNo *pai, MatriculaNo *atual) {
    if (pai == NULL) {
        *raiz = NULL;
    } else if (pai->esquerda == atual) {
        pai->esquerda = NULL;
    } else {
        pai->direita = NULL;
    }
}

void remover_no_com_um_filho(MatriculaNo **raiz, MatriculaNo *pai, MatriculaNo *atual) {
    MatriculaNo *filho = (atual->esquerda != NULL) ? atual->esquerda : atual->direita;

    if (pai == NULL) {
        *raiz = filho;
    } else if (pai->esquerda == atual) {
        pai->esquerda = filho;
    } else {
        pai->direita = filho;
    }
}

void remover_no_com_dois_filhos(MatriculaNo *atual) {
    MatriculaNo *menor_no = atual->direita;
    MatriculaNo *menor_nopai = atual;

    while (menor_no->esquerda != NULL) {
        menor_nopai = menor_no;
        menor_no = menor_no->esquerda;
    }

    atual->codigo_disciplina = menor_no->codigo_disciplina;

    if (menor_nopai->esquerda == menor_no) {
        menor_nopai->esquerda = menor_no->direita;
    } else {
        menor_nopai->direita = menor_no->direita;
    }
}

DisciplinaNo* buscar_disciplina_2(DisciplinaNo *disciplina, int codigo_disciplina) {
    while (disciplina != NULL && disciplina->codigo != codigo_disciplina) {
        if (codigo_disciplina > disciplina->codigo) {
            disciplina = disciplina->direita;
        } else {
            disciplina = disciplina->esquerda;
        }
    }
    return disciplina;
}

NotaNo* buscar_nota(NotaNo *notas, int codigo_disciplina) {
    while (notas != NULL && notas->codigo_disciplina != codigo_disciplina) {
        if (codigo_disciplina > notas->codigo_disciplina) {
            notas = notas->direita;
        } else {
            notas = notas->esquerda;
        }
    }
    return notas;
}

void exibir_informacoes(DisciplinaNo *disciplina_atual, NotaNo *nota_atual) {
    printf("Disciplina: %s\n", disciplina_atual->nome);
    printf("Periodo: %d\n", disciplina_atual->periodo);
    printf("Carga Horaria: %d\n", disciplina_atual->carga_horaria);
    printf("Nota final: %.2f\n", nota_atual->nota_final);
}

void exibir_alunos(AlunoNo *lista) {
    AlunoNo *atual = lista;

    while (atual != NULL) {
        printf("Matricula: %d\n", atual->matricula);
        printf("Nome: %s\n", atual->nome);
        printf("Codigo do curso: %d\n", atual->codigo_curso);

        printf("\n");
        atual = atual->proximo;
    }
}

DisciplinaNo* buscar_disciplina_por_codigo(DisciplinaNo *disciplinas, int codigo) {
    DisciplinaNo *aux;
    if (disciplinas != NULL) {
        if (codigo == disciplinas->codigo) {
            aux = disciplinas;
        } else if (codigo < disciplinas->codigo) {
            aux = buscar_disciplina_por_codigo(disciplinas->esquerda, codigo);
        } else {
            aux = buscar_disciplina_por_codigo(disciplinas->direita, codigo);
        }
    }

    return aux;
}
