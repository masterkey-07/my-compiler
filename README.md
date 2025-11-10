# Definição de C-

## Convenções Léxicas de C-

**Palavras Chaves**

*else if int return void while*

**Símbolos Especiais**

\+ \- \* / < <= > >= == != = ; , () [] {} /\* \*/

**Marcadores de *ID* e *NUM***

ID = letra letra*
NUM = digito digito*
letra = a|..|z|A|..|Z
digito = 0|..|9

**Espaço em branco**

Espaço em branco é composto por brancos, mudanças de linhas e tabulações. O espaço em branco é ignorado, exceto como separador de ***IDs***, ***NUMs*** e palavras-chaves.

**Comentários**

Commentários são cercados pela notação usual de C /\*...\*/. Os comentários podem ser colocados em qualquer lugar que possa ser ocupado por um espaço em branco (ou seja, comentários não podem ser colocados dentro de marcadores), e podem incluir mais de uma linha. Comentários não podem ser aninhados

## Sintaxe e Semântica de C-

Gramática em BNF



$$
\begin{align*}
1. \quad & \text{programa} \rightarrow \text{declaração-lista} \\
2. \quad & \text{declaração-lista} \rightarrow \text{declaração-lista declaração} \mid \text{declaração} \\
3. \quad & \text{declaração} \rightarrow \text{var-declaração} \mid \text{fun-declaração} \\
4. \quad & \text{var-declaração} \rightarrow \text{tipo-especificador } \text{ID} \mid \text{tipo-especificador } \text{ID} \ [ \ \text{NUM} \ ] \, ; \\
5. \quad & \text{tipo-especificador} \rightarrow \text{int} \mid \text{void} \\
6. \quad & \text{fun-declaração} \rightarrow \text{tipo-especificador } \text{ID} ( \text{params} ) \text{ composto-decl} \\
7. \quad & \text{params} \rightarrow \text{param-lista} \mid \text{void} \\
8. \quad & \text{param-lista} \rightarrow \text{param-lista } , \text{ param} \mid \text{param} \\
9. \quad & \text{param} \rightarrow \text{tipo-especificador } \text{ID} \mid \text{tipo-especificador } \text{ID} [] \\
10. \quad & \text{composto-decl} \rightarrow \{ \text{ local-declarações statement-lista } \} \\
11. \quad & \text{local-declarações} \rightarrow \text{local-declarações var-declaração} \mid \text{vazio} \\
12. \quad & \text{statement-lista} \rightarrow \text{statement-lista statement} \mid \text{vazio} \\
13. \quad & \text{statement} \rightarrow \text{expressão-decl} \mid \text{composto-decl} \mid \text{seleção-decl} \mid \text{iteração-decl} \mid \text{retorno-decl} \\
14. \quad & \text{expressão-decl} \rightarrow \text{expressão } ; \mid ; \\
15. \quad & \text{seleção-decl} \rightarrow \text{if ( expressão ) statement} \mid \text{if ( expressão ) statement else statement} \\
16. \quad & \text{iteração-decl} \rightarrow \text{while ( expressão ) statement} \\
17. \quad & \text{retorno-decl} \rightarrow \text{return } ; \mid \text{return expressão } ; \\
18. \quad & \text{expressão} \rightarrow \text{var = expressão} \mid \text{simples-expressão} \\
19. \quad & \text{var} \rightarrow \text{ID} \mid \text{ID} [ \text{expressão} ] \\
20. \quad & \text{simples-expressão} \rightarrow \text{soma-expressão relacional soma-expressão} \mid \text{soma-expressão} \\
21. \quad & \text{relacional} \rightarrow < \mid \le \mid > \mid \ge \mid == \mid != \\
22. \quad & \text{soma-expressão} \rightarrow \text{soma-expressão soma termo} \mid \text{termo} \\
23. \quad & \text{soma} \rightarrow + \mid - \\
24. \quad & \text{termo} \rightarrow \text{termo mult fator} \mid \text{fator} \\
25. \quad & \text{mult} \rightarrow * \mid / \\
26. \quad & \text{fator} \rightarrow ( \text{expressão} ) \mid \text{var} \mid \text{ativação} \mid \text{NUM} \\
27. \quad & \text{ativação} \rightarrow \text{ID} ( \text{args} ) \\
28. \quad & \text{args} \rightarrow \text{arg-lista} \mid \text{vazio} \\
29. \quad & \text{arg-lista} \rightarrow \text{arg-lista , expressão} \mid \text{expressão}
\end{align*}
$$

Para cada uma dessas regras gramaticais, apresentamos uma breve explicação da semântica associada

$$
\begin{align*}
1. \quad & \text{programa} \rightarrow \text{declaração-lista} \\
2. \quad & \text{declaração-lista} \rightarrow \text{declaração-lista declaração} \mid \text{declaração} \\
3. \quad & \text{declaração} \rightarrow \text{var-declaração} \mid \text{fun-declaração} \\
\end{align*}
$$

Um programa é composto por uma lista (ou sequência) de declarações, que podem ser de funções ou de variáveis, em qualquer ordem. Deve haver pelo menos uma declaração.

As restrições semânticas são as seguintes (elas não ocorrem em C):

todas as variáveis e funções devem ser declaradas antes do uso (isso evita refências para ajustes retroativos).

A última declaração em um program deve ser uma declaração de função, da forma void main(void). Observe que em C- não existem protótipos, assim não feitas distinções entre declarações e definições (como em C).

$$
\begin{align*}
4. \quad & \text{var-declaração} \rightarrow \text{tipo-especificador } \text{ID} \mid \text{tipo-especificador } \text{ID} \ [ \ \text{NUM} \ ] \, ; \\
5. \quad & \text{tipo-especificador} \rightarrow \text{int} \mid \text{void} \\
\end{align*}
$$

Uma declaração de variável declara uma variável simples de tipo inteiro ou uma matriz cujo tipo básico é inteiro, e cujos índices variam de 0...NUM-1. Observe que em C- os únicos tipos básicos são inteiro e vazio. Em uma declaração de variável, apenas o especificador de tipos int pode ser usado. Void é usado em declarações de função. Observe também que apenas uma variável pode ser declarada em cada declaração.


$$
\begin{align*}
6. \quad & \text{fun-declaração} \rightarrow \text{tipo-especificador } \text{ID} ( \text{params} ) \text{ composto-decl} \\
7. \quad & \text{params} \rightarrow \text{param-lista} \mid \text{void} \\
8. \quad & \text{param-lista} \rightarrow \text{param-lista } , \text{ param} \mid \text{param} \\
9. \quad & \text{param} \rightarrow \text{tipo-especificador } \text{ID} \mid \text{tipo-especificador } \text{ID} [] \\
\end{align*}
$$