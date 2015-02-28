%{
#include "lexeme.h"
%}
%%
.|\n {return CHAR;}
