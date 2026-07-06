# Simulação Paralela da Difusão de Calor com o Método de Jacobi

Este repositório apresenta a implementação e a análise de desempenho da simulação da difusão de calor em uma placa bidimensional utilizando o **método iterativo de Jacobi**.

O projeto compara três abordagens de execução:

- **Sequencial** (baseline);
- **OpenMP**, utilizando memória compartilhada;
- **MPI**, utilizando memória distribuída.

O objetivo é avaliar como diferentes modelos de paralelismo impactam o desempenho da aplicação, analisando métricas como tempo de execução, speedup, eficiência e escalabilidade.

---

# Sobre o Projeto

A difusão de calor é um problema clássico da Computação Científica e da Computação de Alto Desempenho (HPC). Neste projeto, a temperatura de uma placa bidimensional é calculada iterativamente utilizando o método de Jacobi, permitindo analisar o comportamento da aplicação em diferentes modelos de paralelização.

A implementação foi desenvolvida em linguagem **C**, utilizando:

- **OpenMP** para paralelismo em memória compartilhada;
- **MPI (Message Passing Interface)** para paralelismo em memória distribuída.

---

# Objetivos

Este projeto tem como principais objetivos:

- Implementar a versão sequencial do algoritmo;
- Desenvolver versões paralelas utilizando OpenMP e MPI;
- Comparar o desempenho entre as três implementações;
- Avaliar o impacto do número de threads e processos;
- Analisar limitações impostas pelo hardware disponível;
- Discutir vantagens e desvantagens de cada modelo de paralelismo.

---

# Estrutura do Projeto

```text
.
├── sequential/        # Implementação sequencial
├── openmp/            # Implementação utilizando OpenMP
├── mpi/               # Implementação utilizando MPI
├── resultados/        # Resultados dos experimentos
├── docs/              # Relatórios e documentação
└── README.md
```

---

# Métricas de Avaliação

Durante os experimentos foram analisados:

- Tempo de execução;
- Speedup;
- Eficiência;
- Escalabilidade;
- Utilização dos núcleos do processador;
- Impacto do Hyper-Threading;
- Sobrecarga de comunicação entre processos (MPI).

**Karoline Farias**

Projeto desenvolvido para a disciplina de **Programação Paralela / Computação de Alto Desempenho**.
