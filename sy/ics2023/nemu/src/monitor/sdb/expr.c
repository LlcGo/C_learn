/***************************************************************************************
 *
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ=1,
  DEC=2,PLUS=3,MUL=4,DIV=5, 
  ZUO=6,YOU=7,NUM=8,OR=9,AND=10,
  LEQ=11,NOTEQ=12,DEREF=13
  /* TODO: Add more token types */
};

 uint32_t eval(int p,int q);


static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", PLUS},         // plus
  {"==", TK_EQ},        // equal
  {"\\-",DEC},
  {"\\*",MUL},
  {"\\/",DIV},
  {"\\(", ZUO},
  {"\\)", YOU},
  {"[0-9]",NUM},
  {"\\<\\=",LEQ},
  {"\\!\\=",NOTEQ},
  {"\\|\\|",OR},
  {"\\&\\&",AND},
  {"\\!",'!'}
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        
        position += substr_len;
          
        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        Token tmptoken;
        switch (rules[i].token_type) {
		   case PLUS:
			   tmptoken.type = PLUS;
			   tokens[nr_token++]= tmptoken;
			   break;
		   case DIV:
			   tmptoken.type = DIV;
			   tokens[nr_token++]= tmptoken;
			   break;
		   case MUL:
			   tmptoken.type = MUL;
			   tokens[nr_token++]= tmptoken;
			   break;
		   case DEC:
			   tmptoken.type = DEC;
                           tokens[nr_token++]= tmptoken;
			   break;
		   case TK_EQ:
			   tmptoken.type = TK_EQ;
			   strcpy(tmptoken.str,"==");
			   tokens[nr_token++]= tmptoken;
			   break;
	   	   case ZUO:
                           tmptoken.type = '(';
			   tokens[nr_token++] = tmptoken;
			   break;
	           case YOU:
			   tmptoken.type = ')';
			   tokens[nr_token++] = tmptoken;
			   break;
		   case NUM:
			   tokens[nr_token].type = NUM;
			   strncpy(tokens[nr_token].str, &e[position - substr_len], substr_len);
			   nr_token++;
		           break;
		   case OR:
			   tokens[nr_token].type = OR;
			   strcpy(tokens[nr_token].str,"||");
			   nr_token++;
		           break;
		   case AND:
			   tokens[nr_token].type = AND;
			   strcpy(tokens[nr_token].str,"&&");
			   nr_token++;
		           break;
		   case NOTEQ:
			   tokens[nr_token].type = NOTEQ;
		           strcpy(tokens[nr_token].str,"!=");
			   nr_token++;
			   break;
		   case '!':
			   tmptoken.type = '!';
		           tokens[nr_token++] = tmptoken;		   
		           break;
		   case LEQ:
			   tokens[nr_token].type = LEQ;
			   strcpy(tokens[nr_token].str,"<=");
			   nr_token++;
		           break;
	           case 256:
	                  break;		   
                     default: 
			   printf("i = %d and No rules is com.\n", i);
			   break;
        }
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}


word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  int i;
  for(i = 0; i < nr_token; i++)
  {
	  if(tokens[i].type == '*' && (i == 0 || tokens[i-1].type == PLUS 
				  || tokens[i-1].type == DEC || tokens[i-1].type == MUL
				  || tokens[i-1].type == DIV ))
	  {
		  tokens[i].type =DEREF;
	  }
  }
  *success = true; 
  /* TODO: Insert codes to evaluate the expression. */
  uint32_t res = 0;

  res = eval(0,nr_token-1);
  printf("值-->%d\n",res);
  return res;
}

bool check_parentheses(int p, int q)
{
   if(tokens[p].type != '(' || tokens[q].type!= ')')
	   return false;
   int l = p, r = q;
   while(l < r)
   {
	   if(tokens[l].type == '(')
	   {
		   if(tokens[r].type == ')')
		   {
			   l++,r--;
			   continue;
		   }
		   else
			   r--;
	   }
	   else if(tokens[l].type == ')')
		   return false;
	   else l++;
   }
   return true;
}

int max(int i,int j)
{
   return (i > j) ? i : j;
}

uint32_t eval(int p,int q){
	if(p > q){
            assert(0);
	    return -1;
	}
	else if(p==q){
          // 肯定是数字
	  return atoi(tokens[p].str);
	}
	else if(check_parentheses(p,q)==true){
		return eval(p + 1, q - 1);
	}
	else {
	  /* We should do more things here. */
	   int op = -1;
	   bool flag = false;
	   for(int i = p; i <= q; i++)
	   {
              if(tokens[i].type == '(')
	      {
		      while(tokens[i].type != ')')
			      i++;
	      }
	      if(!flag && tokens[i].type == OR){
		       flag = true;
		       op = max(op,i);
	      }
	      if(!flag && tokens[i].type == AND){
		       flag = true;
		       op = max(op,i);
	      }
	      if(!flag && tokens[i].type == LEQ){
		       flag = true;
		       op = max(op,i);
	      }
	      if(!flag && tokens[i].type == NOTEQ){
		       flag = true;
		       op = max(op,i);
	      }
	      if(!flag && (tokens[i].type == PLUS || tokens[i].type == DEC))
	      {
		      flag = true;
		      op = max(op,i);
	      }
	      if(!flag && (tokens[i].type == MUL || tokens[i].type == DIV))
	      {
		      op = max(op,i);
	      }
	   }
	
	int op_type = tokens[op].type;

	uint32_t val1 = eval(p,op - 1);
	uint32_t val2 = eval(op + 1,q);
	switch(op_type){
		case PLUS:
			return val1 + val2;
		case DEC:
			return val1 - val2;
		case MUL:
			return val1 * val2;
		case TK_EQ:
		        return val1 == val2;	
		case DIV:
		        if(val2 == 0){
		        	return 0;
			}	
			return val1 / val2;
		case LEQ:
			return val1 <= val2;
		case NOTEQ:
			return val1 != val2;
		case OR:
			return val1 || val2;
		case AND:
		        return val1 && val2;
		default:
			printf("No Op type");
			assert(0);
	}
   }
}
