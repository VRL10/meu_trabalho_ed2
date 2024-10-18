#ifndef CODIGO_FONTE_H
#define CODIGO_FONTE_H

// ---------- Estruturas de Dados ------------

typedef struct disciplina_no {
    int codigo;
    char nome[100];
    int periodo;
    int carga_horaria;
    struct disciplina_no *esquerda;
    struct disciplina_no *direita;
} DisciplinaNo;

typedef struct matricula_no {
    int codigo_disciplina;
    struct matricula_no *esquerda;
    struct matricula_no *direita;
} MatriculaNo;

typedef struct nota_no {
    int codigo_disciplina;
    float semestre;
    float nota_final;
    struct nota_no *esquerda;
    struct nota_no *direita;
} NotaNo;

typedef struct aluno_no {
    int matricula;
    char nome[100];
    int codigo_curso;
    NotaNo *notas;
    MatriculaNo *matriculas;
    struct aluno_no *proximo;
} AlunoNo;

typedef struct curso_no {
    int codigo;
    char nome[100];
    int num_periodos;
    struct curso_no *esquerda;
    struct curso_no *direita;
    DisciplinaNo *disciplinas;
    AlunoNo *alunos;
} CursoNo;

// ---------- Funções Relacionadas a Cursos ------------
void gerar_codigo_disciplina(int *codigo);
CursoNo* buscar_curso_por_codigo(CursoNo *curso, int codigo);
void adicionar_curso(CursoNo **curso, int codigo, const char *nome, int num_periodos);
void mostrar_curso(CursoNo *curso);

// ---------- Funções Relacionadas a Alunos ------------
void adicionar_aluno(AlunoNo **aluno, int matricula, char *nome, int codigo_curso);
void listar_alunos_por_curso(AlunoNo *aluno, int codigo_curso);
AlunoNo* buscar_aluno_no_curso(CursoNo *curso, int codigo_curso, int matricula);
void mostrar_alunos(AlunoNo *alunos);
void consultar_historico_aluno(AlunoNo *aluno, CursoNo *curso, int matricula);

// ---------- Funções Relacionadas a Matrículas ------------
void adicionar_matricula(AlunoNo **aluno, int codigo_disciplina, int matricula);
void remover_matricula_aluno(MatriculaNo **matricula, int codigo_disciplina);
void mostrar_matriculas(MatriculaNo *matriculas);
int verificar_matriculas(AlunoNo *aluno, int codigo_disciplina);

// ---------- Funções Relacionadas a Notas ------------
int adicionar_nota(AlunoNo **aluno, int matricula, int codigo_disciplina, int semestre, float nota_final);
void exibir_notas_aluno_disciplina(AlunoNo *aluno, CursoNo *curso, int matricula, int codigo_disciplina);
void listar_notas_disciplina_por_periodo(AlunoNo *aluno, int periodo, int matricula);
int listar_notas_por_periodo(NotaNo *nota, int periodo);
void mostrar_notas_aluno(AlunoNo *aluno, DisciplinaNo *disciplinas, int periodo);

// ---------- Funções Relacionadas a Disciplinas ------------
void mostrar_disciplinas_por_periodo(CursoNo *curso, int periodo);
int adicionar_disciplina(CursoNo **curso, DisciplinaNo *disciplina, int codigo_curso);
void listar_disciplinas_por_curso(CursoNo *curso, int codigo_curso);
int remover_disciplina_do_curso(CursoNo **curso, AlunoNo *alunos, int codigo_curso, int codigo_disciplina);
void listar_disciplinas_por_aluno(AlunoNo *aluno, CursoNo *curso, int matricula);

// ---------- Funções Auxiliares ------------
void remover_folha_matricula(MatriculaNo **raiz, MatriculaNo *pai, MatriculaNo *atual);
void remover_com_um_filho_matricula(MatriculaNo **raiz, MatriculaNo *pai, MatriculaNo *atual);
void remover_com_dois_filhos_matricula(MatriculaNo *atual);
AlunoNo* localizar_aluno(AlunoNo *aluno, int matricula);
void localizar_matricula(MatriculaNo *matricula, int codigo_disciplina, int *encontrado);

#endif
